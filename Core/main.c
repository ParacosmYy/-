#include "config.h"
#include "app.h"
#include "spwm.h"

/*
 * 程序入口文件。
 *
 * main() 仅负责系统初始化和主循环分发，
 * 具体业务逻辑由应用层模块处理。
 */

void main(void)
{
    spwm_init();
    app_init();

    EA = 1;

    spwm_start();
    app_start();

    while (1) {
        app_task();
    }
}
