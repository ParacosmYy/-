#ifndef __KEY_H__
#define __KEY_H__

#include <REGX52.H>

/*
 * 按键输入模块
 * K1: 频率增加, K2: 频率减少
 * 非阻塞计数消抖, 主循环扫描
 */

/* 按键引脚定义 */
#define KEY_FREQ_UP     P3_3    /* K1: 频率+ (INT1 引脚) */
#define KEY_FREQ_DOWN   P3_2    /* K2: 频率- (INT0 引脚) */

/* 消抖计数阈值 (主循环每次约 2ms, 10次 ≈ 20ms) */
#define KEY_DEBOUNCE_COUNT  10

void Key_Init(void);
void Key_Scan(void);

#endif
