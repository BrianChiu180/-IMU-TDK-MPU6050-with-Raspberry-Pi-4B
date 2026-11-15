
#include <errno.h>
#include <fcntl.h>
#include <gpiod.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h> /*20261108 add for 加入標頭及 ioctl 定義*/


#define GPIOCHIP       "/dev/gpiochip0"
#define GPIO_OFFSET    17
#define SENSOR_DEV     "/dev/mpu6050"
#define MAX_EVENTS     5
#define EVENT_BUFFER   8

/*20261108 add for加入標頭及 ioctl 定義*/
#define MPU6050_IOCTL_BASE 'M'
#define MPU6050_IOCTL_GET_INT_STATUS \
    _IOR(MPU6050_IOCTL_BASE, 0x02, unsigned char)

int main(void)
{
    struct gpiod_chip *chip = NULL;
    struct gpiod_line_settings *settings = NULL;
    struct gpiod_line_config *line_cfg = NULL;
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;
    struct gpiod_edge_event_buffer *event_buf = NULL;
    unsigned int offset = GPIO_OFFSET;
    struct gpiod_edge_event *event;
    int sensor_fd = -1;
    int count = 0;
    int ret = 1;
    int n;
    int i;
    int data[7];
    unsigned char int_status;  /*20261108 add for check mpu6050 INT pin status*/


    sensor_fd = open(SENSOR_DEV, O_RDONLY);
    if (sensor_fd < 0) {
        perror("open /dev/mpu6050");
        goto out;
    }

    chip = gpiod_chip_open(GPIOCHIP);
    settings = gpiod_line_settings_new();
    line_cfg = gpiod_line_config_new();
    req_cfg = gpiod_request_config_new();
    event_buf = gpiod_edge_event_buffer_new(EVENT_BUFFER);

    if (!chip || !settings || !line_cfg || !req_cfg || !event_buf) {
        perror("allocate libgpiod resources");
        goto out;
    }

    if (gpiod_line_settings_set_direction(
            settings, GPIOD_LINE_DIRECTION_INPUT) < 0 ||
        gpiod_line_settings_set_edge_detection(
            settings, GPIOD_LINE_EDGE_RISING) < 0 ||
        gpiod_line_config_add_line_settings(
            line_cfg, &offset, 1, settings) < 0) {
        perror("configure GPIO17");
        goto out;
    }

    gpiod_request_config_set_consumer(req_cfg, "mpu6050-int-demo");
    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    if (!request) {
        perror("request GPIO17");
        goto out;
    }

    printf("Waiting for 5 MPU6050 Data Ready events on GPIO17...\n");

    /*20261108 add for 在每次 GPIO 上升沿讀取感測器資料後、列印事件前加入*/
    if (ioctl(sensor_fd, MPU6050_IOCTL_GET_INT_STATUS, &int_status) < 0) 
    {
       perror("ioctl GET_INT_STATUS");
       ret = 1;
       goto out;
    }

    while (count < MAX_EVENTS) {
        ret = gpiod_line_request_wait_edge_events(request, -1);
        if (ret < 0) {
            perror("wait for GPIO event");
            ret = 1;
            goto out;
        }

        n = gpiod_line_request_read_edge_events(
            request, event_buf, EVENT_BUFFER);
        if (n < 0) {
            perror("read GPIO events");
            ret = 1;
            goto out;
        }

        for (i = 0; i < n && count < MAX_EVENTS; i++) 
        {
            event = gpiod_edge_event_buffer_get_event(event_buf, i);
            if (gpiod_edge_event_get_event_type(event) !=
                GPIOD_EDGE_EVENT_RISING_EDGE)
                continue;

            /*
             * This driver copies data and returns 0 on a successful read.
             */
            if (read(sensor_fd, data, sizeof(data)) < 0) {
                perror("read MPU6050");
                ret = 1;
                goto out;
            }

            count++;
            printf("event %d: GPIO17 rising; "
                   "gx=%d gy=%d gz=%d; ax=%.2fg ay=%.2fg az=%.2fg\n",
                   count, data[0], data[1], data[2],
                   data[3] / 2048.0, data[4] / 2048.0,
                   data[5] / 2048.0);
            /*20261108 add for checking INT_Status and DATA_RDY Status*/
            printf("INT_STATUS=0x%02X DATA_RDY=%u\n",
                   int_status, !!(int_status & 0x01));
        }
    }

    ret = 0;

out:
    if (request)
        gpiod_line_request_release(request);
    if (event_buf)
        gpiod_edge_event_buffer_free(event_buf);
    if (req_cfg)
        gpiod_request_config_free(req_cfg);
    if (line_cfg)
        gpiod_line_config_free(line_cfg);
    if (settings)
        gpiod_line_settings_free(settings);
    if (chip)
        gpiod_chip_close(chip);
    if (sensor_fd >= 0)
        close(sensor_fd);

    return ret;
}
