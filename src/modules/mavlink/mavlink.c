/****************************************************************************
 *
 *   Copyright (c) 2012-2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file mavlink.c
 * Adapter functions expected by the protocol library
 *
 * @author Lorenz Meier <lm@inf.ethz.ch>
 */

#include <nuttx/config.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mavlink_bridge_header.h"
#include <systemlib/param/param.h>
// #include <drivers/drv_hrt.h>
// #include <time.h>
// #include <float.h>
// #include <unistd.h>
// #include <nuttx/sched.h>
// #include <sys/prctl.h>
// #include <termios.h>
// #include <errno.h>
// #include <stdlib.h>
// #include <poll.h>

// 
// #include <systemlib/systemlib.h>
// #include <systemlib/err.h>
// #include <mavlink/mavlink_log.h>
// #include <commander/px4_custom_mode.h>

// #include "waypoints.h"
// #include "orb_topics.h"
// #include "mavlink_hil.h"
// #include "util.h"
// #include "waypoints.h"
// #include "mavlink_parameters.h"

// #include <uORB/topics/mission_result.h>

/* define MAVLink specific parameters */
PARAM_DEFINE_INT32(MAV_SYS_ID, 1);
PARAM_DEFINE_INT32(MAV_COMP_ID, 50);
PARAM_DEFINE_INT32(MAV_TYPE, MAV_TYPE_FIXED_WING);

mavlink_system_t mavlink_system = {
	100,
	50,
	MAV_TYPE_FIXED_WING,
	0,
	0,
	0
}; // System ID, 1-255, Component/Subsystem ID, 1-255

/*
 * Internal function to send the bytes through the right serial port
 */
void
mavlink_send_uart_bytes(mavlink_channel_t channel, const uint8_t *ch, int length)
{
	int uart = -1;

	switch (channel) {
		case MAVLINK_COMM_0:
			uart = Mavlink::get_uart_fd(0);
		break;
		case MAVLINK_COMM_1:
			uart = Mavlink::get_uart_fd(1);
		break;
		case MAVLINK_COMM_2:
			uart = Mavlink::get_uart_fd(2);
		break;
		case MAVLINK_COMM_3:
			uart = Mavlink::get_uart_fd(3);
		break;
		case MAVLINK_COMM_4:
			uart = Mavlink::get_uart_fd(4);
		break;
		case MAVLINK_COMM_5:
			uart = Mavlink::get_uart_fd(5);
		break;
		case MAVLINK_COMM_6:
			uart = Mavlink::get_uart_fd(6);
		break;
	}

	write(uart, ch, (size_t)(sizeof(uint8_t) * length));

}

/*
 * Internal function to give access to the channel status for each channel
 */
extern mavlink_status_t *mavlink_get_channel_status(uint8_t channel)
{
	static mavlink_status_t m_mavlink_status[MAVLINK_COMM_NUM_BUFFERS];
	return &m_mavlink_status[channel];
}

/*
 * Internal function to give access to the channel buffer for each channel
 */
extern mavlink_message_t *mavlink_get_channel_buffer(uint8_t channel)
{
	static mavlink_message_t m_mavlink_buffer[MAVLINK_COMM_NUM_BUFFERS];
	return &m_mavlink_buffer[channel];
}
