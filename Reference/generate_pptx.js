const pptxgen = require("pptxgenjs");
const fs = require("fs");

const pres = new pptxgen();
pres.layout = "LAYOUT_16x9"; // 10" x 5.625"
pres.author = "测控2302";
pres.title = "ST89C52 SPWM波形发生器";

// ── Color Palette ──────────────────────────────────────────
const C = {
  darkBg:    "0D1B2A",
  midBg:     "1B2838",
  cardBg:    "162232",
  accent:    "00BFA5",   // teal
  accentDim: "00897B",
  ice:       "B2DFDB",
  lightBg:   "F0F4F8",
  white:     "FFFFFF",
  textDark:  "1A1A2E",
  textMid:   "546E7A",
  textLight: "CFD8DC",
  divider:   "263850",
  tableHead: "0D1B2A",
  tableOdd:  "E8EDF2",
  tableEven: "FFFFFF",
};

const FONT_TITLE = "Arial Black";
const FONT_BODY  = "Calibri";

// ── Helper ─────────────────────────────────────────────────
function makeShadow() {
  return { type: "outer", color: "000000", blur: 4, offset: 2, angle: 135, opacity: 0.18 };
}
function makeCard(slide, x, y, w, h) {
  slide.addShape(pres.shapes.RECTANGLE, {
    x, y, w, h, fill: { color: C.cardBg },
    shadow: makeShadow(), rectRadius: 0.08,
  });
}
function addSectionTag(slide, text) {
  slide.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 0.25, w: 2.2, h: 0.32,
    fill: { color: C.accent }, rectRadius: 0.04,
  });
  slide.addText(text, {
    x: 0.5, y: 0.25, w: 2.2, h: 0.32,
    fontSize: 10, fontFace: FONT_BODY, color: C.white,
    align: "center", valign: "middle", margin: 0,
  });
}
function addSlideNumber(slide, num, total) {
  slide.addText(`${num} / ${total}`, {
    x: 8.8, y: 5.2, w: 1, h: 0.3,
    fontSize: 9, fontFace: FONT_BODY, color: C.textMid,
    align: "right", margin: 0,
  });
}
function imagePlaceholder(slide, x, y, w, h, label) {
  slide.addShape(pres.shapes.RECTANGLE, {
    x, y, w, h,
    fill: { color: "1A2D3D", transparency: 60 },
    line: { color: C.accent, width: 1, dashType: "dash" }, rectRadius: 0.06,
  });
  slide.addText("📷  " + label, {
    x, y, w, h,
    fontSize: 11, fontFace: FONT_BODY, color: C.ice,
    align: "center", valign: "middle", margin: 0,
  });
}

const TOTAL = 12;

// ============================================================
// SLIDE 1 — Title (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };

  // Decorative accent bar
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0, y: 0, w: 0.12, h: 5.625, fill: { color: C.accent },
  });

  // Top thin line
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.12, y: 1.6, w: 9.88, h: 0.003, fill: { color: C.accent },
  });

  s.addText("ST89C52 SPWM 波形发生器", {
    x: 0.6, y: 1.8, w: 8.8, h: 1.2,
    fontSize: 40, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });
  s.addText("课程设计汇报", {
    x: 0.6, y: 2.95, w: 8.8, h: 0.6,
    fontSize: 20, fontFace: FONT_BODY, color: C.accent,
    margin: 0,
  });

  // Divider
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.6, y: 3.7, w: 2.5, h: 0.025, fill: { color: C.divider },
  });

  s.addText([
    { text: "班级：测控2302", options: { breakLine: true, fontSize: 13 } },
    { text: "组员：李文昊(23062048)  古正瑞(23062051)", options: { breakLine: true, fontSize: 13 } },
    { text: "          宋铭威(23062052)  刘  东(23062053)", options: { breakLine: true, fontSize: 13 } },
    { text: "学期：2025-2026-2", options: { fontSize: 13 } },
  ], {
    x: 0.6, y: 3.9, w: 8, h: 1.5,
    fontFace: FONT_BODY, color: C.textLight, lineSpacingMultiple: 1.5, margin: 0,
  });

  addSlideNumber(s, 1, TOTAL);
}

// ============================================================
// SLIDE 2 — Design Background (light)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.lightBg };
  addSectionTag(s, "设计背景与目的");

  s.addText("设计背景与目的", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.textDark,
    bold: true, margin: 0,
  });

  // Two column cards
  makeCard(s, 0.5, 1.6, 4.2, 3.4);
  s.background = { color: C.lightBg };

  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 1.6, w: 4.2, h: 3.4,
    fill: { color: C.white }, shadow: makeShadow(), rectRadius: 0.08,
  });
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 1.6, w: 0.08, h: 3.4, fill: { color: C.accent },
  });
  s.addText("核心目标", {
    x: 0.85, y: 1.75, w: 3.6, h: 0.4,
    fontSize: 16, fontFace: FONT_BODY, color: C.accent, bold: true, margin: 0,
  });
  s.addText([
    { text: "以 ST89C52 单片机为核心", options: { bullet: true, breakLine: true } },
    { text: "设计 1Hz ~ 20Hz 频率可调 SPWM 波形发生器", options: { bullet: true, breakLine: true } },
    { text: "按键步进调节，数码管实时显示", options: { bullet: true, breakLine: true } },
    { text: "P3.7 直接输出 SPWM 信号", options: { bullet: true } },
  ], {
    x: 0.85, y: 2.3, w: 3.6, h: 2.5,
    fontSize: 12, fontFace: FONT_BODY, color: C.textDark, lineSpacingMultiple: 1.4, margin: 0,
  });

  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.3, y: 1.6, w: 4.2, h: 3.4,
    fill: { color: C.white }, shadow: makeShadow(), rectRadius: 0.08,
  });
  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.3, y: 1.6, w: 0.08, h: 3.4, fill: { color: C.accent },
  });
  s.addText("能力培养", {
    x: 5.65, y: 1.75, w: 3.6, h: 0.4,
    fontSize: 16, fontFace: FONT_BODY, color: C.accent, bold: true, margin: 0,
  });
  s.addText([
    { text: "掌握 SPWM 波形生成原理", options: { bullet: true, breakLine: true } },
    { text: "实践定时器中断与查表法", options: { bullet: true, breakLine: true } },
    { text: "硬件电路搭建与调试", options: { bullet: true, breakLine: true } },
    { text: "提升系统级设计与工程实践能力", options: { bullet: true } },
  ], {
    x: 5.65, y: 2.3, w: 3.6, h: 2.5,
    fontSize: 12, fontFace: FONT_BODY, color: C.textDark, lineSpacingMultiple: 1.4, margin: 0,
  });

  addSlideNumber(s, 2, TOTAL);
}

// ============================================================
// SLIDE 3 — SPWM Principle (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };
  addSectionTag(s, "原理概述");

  s.addText("SPWM 原理概述", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  s.addText([
    { text: "SPWM（正弦脉宽调制）通过对脉冲宽度进行正弦规律调制，", options: { breakLine: true } },
    { text: "使输出波形等效为正弦波。", options: { breakLine: true, fontSize: 12 } },
  ], {
    x: 0.5, y: 1.4, w: 5, h: 0.7,
    fontSize: 13, fontFace: FONT_BODY, color: C.textLight, margin: 0,
  });

  // Principle steps cards
  const steps = [
    ["01", "查表法", "100点正弦表存ROM\n占空比 0~99 级"],
    ["02", "双相中断", "Timer0 Mode1\nLOW相 + HIGH相"],
    ["03", "频率调节", "采样周期查表\n1~20Hz 按键步进"],
  ];
  steps.forEach(([num, title, desc], i) => {
    const cx = 0.5 + i * 3.15;
    s.addShape(pres.shapes.RECTANGLE, {
      x: cx, y: 2.2, w: 2.9, h: 2.6,
      fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.08,
    });
    s.addShape(pres.shapes.RECTANGLE, {
      x: cx, y: 2.2, w: 2.9, h: 0.06, fill: { color: C.accent },
    });
    s.addText(num, {
      x: cx + 0.15, y: 2.4, w: 0.5, h: 0.5,
      fontSize: 28, fontFace: FONT_TITLE, color: C.accent, margin: 0,
    });
    s.addText(title, {
      x: cx + 0.7, y: 2.45, w: 2, h: 0.4,
      fontSize: 16, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
    });
    s.addText(desc, {
      x: cx + 0.15, y: 3.1, w: 2.6, h: 1.5,
      fontSize: 12, fontFace: FONT_BODY, color: C.textLight, lineSpacingMultiple: 1.4, margin: 0,
    });
  });

  // Formula
  s.addText("duty[i] = round( (sin(2π·i/100) + 1) / 2 × 99 )", {
    x: 0.5, y: 5.0, w: 9, h: 0.4,
    fontSize: 12, fontFace: "Consolas", color: C.ice, align: "center", margin: 0,
  });

  addSlideNumber(s, 3, TOTAL);
}

// ============================================================
// SLIDE 4 — System Architecture Overview (light)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.lightBg };
  addSectionTag(s, "系统架构");

  s.addText("系统总体架构", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.textDark,
    bold: true, margin: 0,
  });

  // Center MCU block
  s.addShape(pres.shapes.RECTANGLE, {
    x: 3.5, y: 1.6, w: 3, h: 1.2,
    fill: { color: C.darkBg }, shadow: makeShadow(), rectRadius: 0.06,
  });
  s.addText("ST89C52\n主控制器", {
    x: 3.5, y: 1.6, w: 3, h: 1.2,
    fontSize: 15, fontFace: FONT_BODY, color: C.white,
    align: "center", valign: "middle", bold: true, margin: 0,
  });

  // Module blocks around MCU
  const modules = [
    { x: 0.5, y: 1.7, w: 2.4, h: 1.0, title: "按键输入", desc: "K1/K2 频率±", color: "00897B" },
    { x: 7.1, y: 1.7, w: 2.4, h: 1.0, title: "数码管显示", desc: "两位共阳 LED", color: "1565C0" },
    { x: 3.5, y: 3.3, w: 3.0, h: 1.0, title: "SPWM 输出", desc: "P3.7 PWM 信号", color: "C62828" },
    { x: 0.5, y: 3.3, w: 2.4, h: 1.0, title: "定时器中断", desc: "Timer0 双相", color: "E65100" },
    { x: 7.1, y: 3.3, w: 2.4, h: 1.0, title: "正弦查找表", desc: "100 点 ROM", color: "6A1B9A" },
  ];

  modules.forEach(m => {
    s.addShape(pres.shapes.RECTANGLE, {
      x: m.x, y: m.y, w: m.w, h: m.h,
      fill: { color: C.white }, shadow: makeShadow(), rectRadius: 0.06,
    });
    s.addShape(pres.shapes.RECTANGLE, {
      x: m.x, y: m.y, w: m.w, h: 0.06, fill: { color: m.color },
    });
    s.addText(m.title, {
      x: m.x + 0.15, y: m.y + 0.15, w: m.w - 0.3, h: 0.35,
      fontSize: 13, fontFace: FONT_BODY, color: C.textDark, bold: true, margin: 0,
    });
    s.addText(m.desc, {
      x: m.x + 0.15, y: m.y + 0.5, w: m.w - 0.3, h: 0.35,
      fontSize: 11, fontFace: FONT_BODY, color: C.textMid, margin: 0,
    });
  });

  // Connecting lines (arrows as lines)
  // Left → MCU
  s.addShape(pres.shapes.LINE, {
    x: 2.9, y: 2.2, w: 0.6, h: 0,
    line: { color: C.textMid, width: 1.5 },
  });
  // MCU → Right
  s.addShape(pres.shapes.LINE, {
    x: 6.5, y: 2.2, w: 0.6, h: 0,
    line: { color: C.textMid, width: 1.5 },
  });
  // MCU → SPWM (down)
  s.addShape(pres.shapes.LINE, {
    x: 5.0, y: 2.8, w: 0, h: 0.5,
    line: { color: C.textMid, width: 1.5 },
  });
  // Timer → MCU (left-down to center)
  s.addShape(pres.shapes.LINE, {
    x: 2.9, y: 3.5, w: 0.6, h: -0.7,
    line: { color: C.textMid, width: 1, dashType: "dash" },
  });
  // Sine table → MCU
  s.addShape(pres.shapes.LINE, {
    x: 7.1, y: 3.5, w: -0.6, h: -0.7,
    line: { color: C.textMid, width: 1, dashType: "dash" },
  });

  addSlideNumber(s, 4, TOTAL);
}

// ============================================================
// SLIDE 5 — Hardware Design (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };
  addSectionTag(s, "硬件设计");

  s.addText("硬件设计与引脚分配", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  // Pin assignment table
  const pinHeaders = [
    { text: "单片机引脚", options: { fill: { color: C.accentDim }, color: C.white, bold: true, fontSize: 11, align: "center" } },
    { text: "功能", options: { fill: { color: C.accentDim }, color: C.white, bold: true, fontSize: 11, align: "center" } },
    { text: "宏定义", options: { fill: { color: C.accentDim }, color: C.white, bold: true, fontSize: 11, align: "center" } },
    { text: "连接说明", options: { fill: { color: C.accentDim }, color: C.white, bold: true, fontSize: 11, align: "center" } },
  ];
  const pinData = [
    pinHeaders,
    ["P1.0~P1.6", "七段数码管 a~g", "SEG_PORT", "段选，低电平点亮"],
    ["P2.0", "数码管十位位选", "DIG_TENS", "低电平选中"],
    ["P2.1", "数码管个位位选", "DIG_ONES", "低电平选中"],
    ["P3.2", "K2 按键 (频率-)", "KEY_FREQ_DOWN", "低电平有效"],
    ["P3.3", "K1 按键 (频率+)", "KEY_FREQ_UP", "低电平有效"],
    ["P3.7", "SPWM 波形输出", "SPWM_PIN", "接示波器/负载"],
  ].map((row, ri) => row.map((cell, ci) => {
    if (ri === 0) return cell;
    return { text: cell, options: { fill: { color: ri % 2 === 0 ? "1A2D3D" : "14243A" }, color: C.textLight, fontSize: 10, align: "center", valign: "middle" } };
  }));

  s.addTable(pinData, {
    x: 0.5, y: 1.5, w: 9, colW: [1.8, 2.2, 2.0, 3.0],
    border: { pt: 0.5, color: C.divider },
    rowH: [0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35],
  });

  // Image placeholder for circuit diagram
  imagePlaceholder(s, 5.5, 4.1, 4, 1.2, "[引脚连接示意图 / 最小系统原理图]");

  // Key components callout
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 4.1, w: 4.5, h: 1.2,
    fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.06,
  });
  s.addText("核心器件", {
    x: 0.7, y: 4.15, w: 2, h: 0.3,
    fontSize: 12, fontFace: FONT_BODY, color: C.accent, bold: true, margin: 0,
  });
  s.addText([
    { text: "ST89C52 @ 11.0592MHz", options: { bullet: true, breakLine: true } },
    { text: "两位共阳七段 LED 数码管", options: { bullet: true, breakLine: true } },
    { text: "独立按键 K1 / K2", options: { bullet: true } },
  ], {
    x: 0.7, y: 4.45, w: 4.1, h: 0.8,
    fontSize: 10, fontFace: FONT_BODY, color: C.textLight, lineSpacingMultiple: 1.3, margin: 0,
  });

  addSlideNumber(s, 5, TOTAL);
}

// ============================================================
// SLIDE 6 — Software Architecture (light)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.lightBg };
  addSectionTag(s, "软件架构");

  s.addText("软件模块架构", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.textDark,
    bold: true, margin: 0,
  });

  // Module tree
  const mods = [
    { x: 3.5, y: 1.5, w: 3, h: 0.7, title: "main.c", desc: "主程序入口 · 全局调度", color: C.darkBg, textColor: C.white },
    { x: 0.3, y: 2.9, w: 2.2, h: 1.4, title: "spwm.c", desc: "100点正弦表\nTimer0 双相中断\nSPWM 生成", color: "C62828", textColor: C.white },
    { x: 2.7, y: 2.9, w: 2.2, h: 1.4, title: "display.c", desc: "共阳七段码\n动态扫描显示\n每位 2ms 刷新", color: "1565C0", textColor: C.white },
    { x: 5.1, y: 2.9, w: 2.2, h: 1.4, title: "key.c", desc: "按键扫描\n20ms 消抖\n频率 ±1Hz", color: "00897B", textColor: C.white },
    { x: 7.5, y: 2.9, w: 2.2, h: 1.4, title: "delay.c", desc: "毫秒/微秒\n软件延时\n11.0592MHz", color: "E65100", textColor: C.white },
  ];

  mods.forEach((m, i) => {
    s.addShape(pres.shapes.RECTANGLE, {
      x: m.x, y: m.y, w: m.w, h: m.h,
      fill: { color: i === 0 ? C.darkBg : C.white }, shadow: makeShadow(), rectRadius: 0.06,
    });
    s.addShape(pres.shapes.RECTANGLE, {
      x: m.x, y: m.y, w: m.w, h: 0.06,
      fill: { color: m.color },
    });
    if (i > 0) {
      s.addShape(pres.shapes.RECTANGLE, {
        x: m.x, y: m.y, w: m.w, h: 0.45,
        fill: { color: m.color, transparency: 10 }, rectRadius: 0,
      });
    }
    s.addText(m.title, {
      x: m.x + 0.1, y: m.y + 0.08, w: m.w - 0.2, h: 0.35,
      fontSize: i === 0 ? 15 : 13, fontFace: FONT_BODY,
      color: i === 0 ? C.white : C.white, bold: true, margin: 0,
    });
    if (i > 0) {
      s.addText(m.desc, {
        x: m.x + 0.1, y: m.y + 0.55, w: m.w - 0.2, h: 0.8,
        fontSize: 10, fontFace: FONT_BODY, color: C.textDark, lineSpacingMultiple: 1.3, margin: 0,
      });
    } else {
      s.addText(m.desc, {
        x: m.x + 0.1, y: m.y + 0.35, w: m.w - 0.2, h: 0.3,
        fontSize: 10, fontFace: FONT_BODY, color: C.accent, margin: 0,
      });
    }
  });

  // Connecting lines from main to modules
  for (let i = 0; i < 4; i++) {
    const mx = mods[i + 1].x + mods[i + 1].w / 2;
    s.addShape(pres.shapes.LINE, {
      x: mx, y: 2.2, w: 0, h: 0.7,
      line: { color: C.textMid, width: 1, dashType: "dash" },
    });
  }

  addSlideNumber(s, 6, TOTAL);
}

// ============================================================
// SLIDE 7 — SPWM Generation Detail (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };
  addSectionTag(s, "核心算法");

  s.addText("SPWM 生成 — 双相中断机制", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 28, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  // LOW phase card
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 1.5, w: 4.3, h: 2.2,
    fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.08,
  });
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.5, y: 1.5, w: 4.3, h: 0.5,
    fill: { color: "1565C0" }, rectRadius: 0,
  });
  s.addText("LOW 相 (phase=0)", {
    x: 0.65, y: 1.52, w: 3.5, h: 0.45,
    fontSize: 14, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
  });
  s.addText([
    { text: "SPWM_PIN = 0 (输出低电平)", options: { breakLine: true } },
    { text: "读 sine_table[index] → duty", options: { breakLine: true } },
    { text: "ticks = T × (100 - duty) / 100", options: { breakLine: true } },
    { text: "装载定时器 → phase = 1", options: {} },
  ], {
    x: 0.65, y: 2.1, w: 4.0, h: 1.5,
    fontSize: 11, fontFace: "Consolas", color: C.ice, lineSpacingMultiple: 1.4, margin: 0,
  });

  // HIGH phase card
  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.2, y: 1.5, w: 4.3, h: 2.2,
    fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.08,
  });
  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.2, y: 1.5, w: 4.3, h: 0.5,
    fill: { color: "C62828" }, rectRadius: 0,
  });
  s.addText("HIGH 相 (phase=1)", {
    x: 5.35, y: 1.52, w: 3.5, h: 0.45,
    fontSize: 14, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
  });
  s.addText([
    { text: "SPWM_PIN = 1 (输出高电平)", options: { breakLine: true } },
    { text: "ticks = T × duty / 100", options: { breakLine: true } },
    { text: "装载定时器", options: { breakLine: true } },
    { text: "index++ → phase = 0", options: {} },
  ], {
    x: 5.35, y: 2.1, w: 4.0, h: 1.5,
    fontSize: 11, fontFace: "Consolas", color: C.ice, lineSpacingMultiple: 1.4, margin: 0,
  });

  // Arrow between
  s.addText("→", {
    x: 4.6, y: 2.1, w: 0.8, h: 0.5,
    fontSize: 24, fontFace: FONT_BODY, color: C.accent, align: "center", valign: "middle", margin: 0,
  });

  // Image placeholder for waveform
  imagePlaceholder(s, 0.5, 4.0, 9.0, 1.3, "[SPWM 波形仿真截图 — 示波器 / Keil 仿真]");

  addSlideNumber(s, 7, TOTAL);
}

// ============================================================
// SLIDE 8 — Key Parameters (light)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.lightBg };
  addSectionTag(s, "技术参数");

  s.addText("关键技术参数", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.textDark,
    bold: true, margin: 0,
  });

  const paramHeaders = [
    { text: "参数", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
    { text: "值", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
    { text: "说明", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
  ];

  const paramRows = [
    ["晶振频率", "11.0592 MHz", "标准波特率晶振"],
    ["机器周期", "1.085 μs", "12 时钟 / 机器周期"],
    ["正弦采样点", "100 点/周期", "角度间隔 3.6°"],
    ["PWM 分辨率", "100 级 (0~99)", "占空比精度"],
    ["频率范围", "1 ~ 20 Hz", "按键 1Hz 步进"],
    ["中断频率 @10Hz", "2000 次/秒", "100×2×10"],
    ["CPU 占用 @10Hz", "~8.7%", "ISR 开销极低"],
    ["最小相位钳位", "80 机器周期", "防止 ISR 执行不完"],
  ];

  const tableData = [
    paramHeaders,
    ...paramRows.map((row, ri) => row.map(cell => ({
      text: cell,
      options: {
        fill: { color: ri % 2 === 0 ? C.tableOdd : C.tableEven },
        color: C.textDark, fontSize: 11, align: "center", valign: "middle",
      },
    }))),
  ];

  s.addTable(tableData, {
    x: 0.5, y: 1.5, w: 9, colW: [2.5, 2.5, 4.0],
    border: { pt: 0.5, color: "B0BEC5" },
    rowH: [0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38, 0.38],
  });

  addSlideNumber(s, 8, TOTAL);
}

// ============================================================
// SLIDE 9 — Frequency Regulation (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };
  addSectionTag(s, "按键调频");

  s.addText("频率调节与显示", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  // Flow diagram
  const flowSteps = [
    { x: 0.4, y: 1.6, w: 2.8, h: 0.9, title: "按键扫描", desc: "K1/K2 状态检测", color: "00897B" },
    { x: 3.6, y: 1.6, w: 2.8, h: 0.9, title: "消抖处理", desc: "20ms 延时确认", color: "1565C0" },
    { x: 6.8, y: 1.6, w: 2.8, h: 0.9, title: "频率修改", desc: "g_frequency ±1", color: "C62828" },
  ];

  flowSteps.forEach((step, i) => {
    s.addShape(pres.shapes.RECTANGLE, {
      x: step.x, y: step.y, w: step.w, h: step.h,
      fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.06,
    });
    s.addShape(pres.shapes.RECTANGLE, {
      x: step.x, y: step.y, w: step.w, h: 0.05, fill: { color: step.color },
    });
    s.addText(step.title, {
      x: step.x + 0.1, y: step.y + 0.1, w: step.w - 0.2, h: 0.35,
      fontSize: 13, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
    });
    s.addText(step.desc, {
      x: step.x + 0.1, y: step.y + 0.45, w: step.w - 0.2, h: 0.3,
      fontSize: 10, fontFace: FONT_BODY, color: C.textLight, margin: 0,
    });
    if (i < 2) {
      s.addText("→", {
        x: step.x + step.w, y: step.y, w: 0.8, h: 0.9,
        fontSize: 20, fontFace: FONT_BODY, color: C.accent, align: "center", valign: "middle", margin: 0,
      });
    }
  });

  // Display flow
  const dispSteps = [
    { x: 0.4, y: 3.0, w: 4.4, h: 0.9, title: "数码管动态扫描", desc: "十位 → 个位 交替 2ms 刷新，消鬼影", color: "6A1B9A" },
    { x: 5.2, y: 3.0, w: 4.4, h: 0.9, title: "ISR 自动生效", desc: "下次中断自动读取新 g_frequency", color: "E65100" },
  ];

  dispSteps.forEach(step => {
    s.addShape(pres.shapes.RECTANGLE, {
      x: step.x, y: step.y, w: step.w, h: step.h,
      fill: { color: C.cardBg }, shadow: makeShadow(), rectRadius: 0.06,
    });
    s.addShape(pres.shapes.RECTANGLE, {
      x: step.x, y: step.y, w: step.w, h: 0.05, fill: { color: step.color },
    });
    s.addText(step.title, {
      x: step.x + 0.1, y: step.y + 0.1, w: step.w - 0.2, h: 0.35,
      fontSize: 13, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
    });
    s.addText(step.desc, {
      x: step.x + 0.1, y: step.y + 0.45, w: step.w - 0.2, h: 0.3,
      fontSize: 10, fontFace: FONT_BODY, color: C.textLight, margin: 0,
    });
  });

  // Image placeholder
  imagePlaceholder(s, 0.5, 4.2, 9.0, 1.1, "[不同频率下的 SPWM 波形对比 — 1Hz / 5Hz / 10Hz / 20Hz]");

  addSlideNumber(s, 9, TOTAL);
}

// ============================================================
// SLIDE 10 — Seven Segment Display Table (light)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.lightBg };
  addSectionTag(s, "显示编码");

  s.addText("共阳七段码编码表", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.textDark,
    bold: true, margin: 0,
  });

  const segHeaders = [
    { text: "数字", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
    { text: "编码 (HEX)", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
    { text: "点亮的段", options: { fill: { color: C.darkBg }, color: C.white, bold: true, fontSize: 12, align: "center" } },
  ];

  const segData = [
    ["0", "0xC0", "a b c d e f"],
    ["1", "0xF9", "b c"],
    ["2", "0xA4", "a b d e g"],
    ["3", "0xB0", "a b c d g"],
    ["4", "0x99", "b c f g"],
    ["5", "0x92", "a c d f g"],
    ["6", "0x82", "a c d e f g"],
    ["7", "0xF8", "a b c"],
    ["8", "0x80", "a b c d e f g"],
    ["9", "0x90", "a b c d f g"],
  ];

  const tableData = [
    segHeaders,
    ...segData.map((row, ri) => row.map(cell => ({
      text: cell,
      options: {
        fill: { color: ri % 2 === 0 ? C.tableOdd : C.tableEven },
        color: C.textDark, fontSize: 11, align: "center", valign: "middle",
      },
    }))),
  ];

  s.addTable(tableData, {
    x: 0.5, y: 1.5, w: 5, colW: [1.0, 1.5, 2.5],
    border: { pt: 0.5, color: "B0BEC5" },
    rowH: [0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35, 0.35],
  });

  // Image placeholder for LED display circuit
  imagePlaceholder(s, 6, 1.5, 3.5, 2.5, "[LED 显示电路原理图]");

  // Display scan explanation
  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.8, y: 4.2, w: 4, h: 1.0,
    fill: { color: C.white }, shadow: makeShadow(), rectRadius: 0.06,
  });
  s.addShape(pres.shapes.RECTANGLE, {
    x: 5.8, y: 4.2, w: 0.06, h: 1.0, fill: { color: C.accent },
  });
  s.addText("动态扫描流程", {
    x: 6.05, y: 4.25, w: 3.5, h: 0.3,
    fontSize: 12, fontFace: FONT_BODY, color: C.accent, bold: true, margin: 0,
  });
  s.addText("熄灭 → 段选 → 位选 → 延时 2ms → 交替", {
    x: 6.05, y: 4.55, w: 3.5, h: 0.5,
    fontSize: 10, fontFace: FONT_BODY, color: C.textDark, margin: 0,
  });

  addSlideNumber(s, 10, TOTAL);
}

// ============================================================
// SLIDE 11 — Physical Demo (dark, image placeholders)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };
  addSectionTag(s, "实物展示");

  s.addText("实物展示与运行效果", {
    x: 0.5, y: 0.7, w: 9, h: 0.6,
    fontSize: 30, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  // 2x2 grid of image placeholders
  imagePlaceholder(s, 0.5, 1.5, 4.3, 1.7, "[系统整体实物照片]");
  imagePlaceholder(s, 5.2, 1.5, 4.3, 1.7, "[各模块特写照片]");
  imagePlaceholder(s, 0.5, 3.5, 4.3, 1.7, "[数码管显示频率 + 示波器波形]");
  imagePlaceholder(s, 5.2, 3.5, 4.3, 1.7, "[完整电路原理图]");

  addSlideNumber(s, 11, TOTAL);
}

// ============================================================
// SLIDE 12 — Summary (dark)
// ============================================================
{
  const s = pres.addSlide();
  s.background = { color: C.darkBg };

  // Accent bar
  s.addShape(pres.shapes.RECTANGLE, {
    x: 0, y: 0, w: 0.12, h: 5.625, fill: { color: C.accent },
  });

  s.addShape(pres.shapes.RECTANGLE, {
    x: 0.12, y: 1.4, w: 9.88, h: 0.003, fill: { color: C.accent },
  });

  s.addText("总结与展望", {
    x: 0.6, y: 1.6, w: 8.8, h: 0.8,
    fontSize: 36, fontFace: FONT_TITLE, color: C.white,
    bold: true, margin: 0,
  });

  // Summary cards
  const items = [
    ["查表法 + 双相中断", "实现 1~20Hz 可调 SPWM 输出"],
    ["按键调频 + 数码管显示", "人机交互简洁直观"],
    ["模块化软件架构", "SPWM / Display / Key / Delay 独立解耦"],
    ["扩展方向", "增加 LCD 显示、多路输出、更高频率范围"],
  ];

  items.forEach(([title, desc], i) => {
    const cy = 2.6 + i * 0.7;
    s.addShape(pres.shapes.RECTANGLE, {
      x: 0.6, y: cy, w: 0.08, h: 0.5, fill: { color: C.accent },
    });
    s.addText(title, {
      x: 0.85, y: cy, w: 3.5, h: 0.25,
      fontSize: 13, fontFace: FONT_BODY, color: C.white, bold: true, margin: 0,
    });
    s.addText(desc, {
      x: 0.85, y: cy + 0.25, w: 8, h: 0.25,
      fontSize: 11, fontFace: FONT_BODY, color: C.textLight, margin: 0,
    });
  });

  s.addText("感谢聆听", {
    x: 0.6, y: 4.9, w: 8.8, h: 0.5,
    fontSize: 18, fontFace: FONT_BODY, color: C.accent,
    align: "center", margin: 0,
  });

  addSlideNumber(s, 12, TOTAL);
}

// ── Write File ─────────────────────────────────────────────
const outPath = "c:/D/Workplace/C51/Reference/SPWM波形发生器汇报.pptx";
pres.writeFile({ fileName: outPath }).then(() => {
  console.log("PPT generated: " + outPath);
}).catch(err => {
  console.error("Error:", err);
});
