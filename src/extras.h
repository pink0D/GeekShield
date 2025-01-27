//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _EXTRAS_H_
#define _EXTRAS_H_

#define TASK_STACK_SIZE 8*1024
#define TASK_DEFAULT_DELAY_MILLIS 50

#define MAX_PROFILES      4
#define MAX_GEEKSERVOS    5

#define vTaskDelayMillis(m) vTaskDelay(pdMS_TO_TICKS(m));

#endif