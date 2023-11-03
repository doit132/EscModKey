# 项目介绍

如果你使用过 AHK 或曾经遭遇过修饰键 (ctrl, alt, shift, win) 出现按键黏滞, 那么这个项目就是为了解决这个问题

- 什么是按键黏滞:
  - 物理按键已经松开, 但是它仍然在发挥作用
  - 例如: alt 出现黏滞, 即 alt 实体按键已经松开, 按理说它应该不会再发挥作用, 但此时我按下 tab 就变成了 alt-tab 激活了切换软件窗口的功能

- 为什么会出现按键黏滞这个问题?
  - 主要是因为**模拟按键**没有释放
  - 比如: 我经常使用 ahk 来写一些按键模拟的脚本, 就经常遇到按键黏滞的现象, 我也使用鼠标手势, 它也能实现模拟按键, 也就是说鼠标手势也有可能引起按键黏滞

## 使用到的依赖

- 使用到了 [interception](https://github.com/oblitum/Interception) 驱动项目