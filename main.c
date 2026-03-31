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
    SPWM_Init();
    App_Init();

    EA = 1;

    SPWM_Start();
    App_Start();

    while (1) {
        App_Task();
    }
}
