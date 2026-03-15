# MonoGUI 故障排除指南

## 常见编译错误及解决方案

### ❌ 错误 1: `redefinition of 'class Adafruit_SH1106'`

```
error: redefinition of 'class Adafruit_SH1106'
  141 | class Adafruit_SH1106 : public Adafruit_GFX {
```

**原因：**
重复包含了 SH1106 库的头文件。

**解决方案：**

1. **检查你的代码包含顺序：**
```cpp
// ✅ 正确的顺序：
#include <Adafruit_SH1106.h>  // 只包含一次
#include <MonoGUI.h>

// ❌ 错误的顺序：
#include <MonoGUI.h>
#include <Adafruit_SH1106.h>  // MonoGUI 已经包含过了
```

2. **只包含一个显示库：**
```cpp
// ✅ 正确：选择一个
#include <Adafruit_SH1106.h>
#include <MonoGUI.h>

// ❌ 错误：不要同时包含多个
#include <Adafruit_SSD1306.h>
#include <Adafruit_SH1106.h>  // 冲突！
#include <MonoGUI.h>
```

3. **检查 PlatformIO 依赖：**
在 `platformio.ini` 中：
```ini
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SH1106    # 只选择一个
    # adafruit/Adafruit SSD1306  # 不要同时使用两个
```

---

### ❌ 错误 2: `cannot convert 'Adafruit_SH1106' to 'Adafruit_SSD1306&'`

```
error: cannot convert 'Adafruit_SH1106' to 'Adafruit_SSD1306&'
```

**原因：**
使用了旧版本的 MonoGUI 或没有正确更新库。

**解决方案：**

1. **更新到最新版本的 MonoGUI**
2. **确保使用正确的包含顺序：**
```cpp
#include <Adafruit_SH1106.h>  // 先包含显示库
#include <MonoGUI.h>           // 后包含 MonoGUI
```

3. **清除缓存并重新编译：**
```bash
# PlatformIO
pio run -t clean
pio run

# Arduino IDE
草图 > 验证/编译
```

---

### ❌ 错误 3: `'Adafruit_GFX' file not found`

```
fatal error: Adafruit_GFX.h: No such file or directory
```

**原因：**
缺少 Adafruit GFX 库。

**解决方案：**

在 `platformio.ini` 中添加：
```ini
lib_deps =
    adafruit/Adafruit GFX Library
```

或在 Arduino IDE 中安装：
```
工具 > 管理库 > 搜索 "Adafruit GFX" > 安装
```

---

### ❌ 错误 4: 显示屏不显示任何内容

**可能原因：**

1. **I2C 地址不正确**
   - SSD1306 通常是 `0x3C`
   - SH1106 通常是 `0x3C`
   - 使用 I2C 扫描器确认地址

2. **接线问题**
   - 检查 SDA 和 SCL 连接
   - 确保共地 (GND)
   - 检查电源电压 (3.3V 或 5V)

3. **初始化问题**
```cpp
// SSD1306
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

// SH1106
display.begin(SH1106_I2C_ADDRESS, -1);
```

4. **忘记调用 display.display()**
```cpp
display.clearDisplay();
// 绘制内容...
display.display();  // 必须调用这个！
```

---

### ❌ 错误 5: ESP32-C3 特定问题

使用 `Adafruit_ESP32_SH1106` 库时：

```cpp
// ✅ 使用特定的 ESP32 SH1106 库
#include <Adafruit_ESP32_SH1106.h>
#include <MonoGUI.h>

Adafruit_ESP32_SH1106 display(-1);
```

**注意：** 确保在 `platformio.ini` 中使用正确的库名称：
```ini
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ESP32 SH1106
```

---

## PlatformIO 配置示例

### ESP32 + SSD1306
```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    MonoGUI
```

### ESP32-C3 + SH1106
```ini
[env:esp32-c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit ESP32 SH1106
    MonoGUI
```

### Arduino Uno + SSD1306
```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps =
    adafruit/Adafruit GFX Library
    adafruit/Adafruit SSD1306
    MonoGUI
```

---

## 获取帮助

如果以上解决方案都无法解决问题：

1. 检查示例代码：`examples/` 文件夹
2. 查看详细指南：[SH1106_GUIDE.md](SH1106_GUIDE.md)
3. 提交 Issue：在 GitHub 上报告问题
4. 包含以下信息：
   - 开发板型号
   - PlatformIO/Arduino IDE 版本
   - 完整的错误消息
   - 相关代码片段

---

## 快速检查清单

在报告问题之前，请检查：

- [ ] 显示库在 MonoGUI.h 之前包含
- [ ] 只包含一个显示库（不是多个）
- [ ] Adafruit GFX 库已安装
- [ ] I2C 接线正确
- [ ] 使用正确的 I2C 地址
- [ ] 调用了 `display.begin()`
- [ ] 调用了 `display.display()`
- [ ] 清除了编译缓存并重新编译
