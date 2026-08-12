# 项目上下文

> AI 在处理本项目相关任务时，应优先参考此文件了解背景。

## 项目基本信息

- **项目名称**：庭院灵踪
- **类型**：国风庭院探索解谜游戏
- **目标平台**：PC（首发）→ 移动端（后续）
- **当前阶段**：2D Web 原型验证 + UE5 全 3D 版本预研

## 核心体验

- **世界观/背景**：江南古典园林，玩家扮演误入古园的旅人，通过探索与解谜揭开庭院主人尘封的故事。
- **玩法**：第三人称探索 + 轻量机关解谜 + 物品收集 + 叙事对话。强调"慢下来"的沉浸感。
- **美术风格**：写实国风，参考苏州园林拙政园、留园等。低饱和青灰主色，点缀朱红、鎏金。
- **目标玩家**：喜欢氛围探索、叙事驱动、解谜轻度化的休闲玩家。

## 技术栈

### 2D Web 原型（Asitch 引擎）
- **引擎/框架**：Asitch（自研轻量 2D Canvas 引擎，ES Modules）
- **开发语言**：JavaScript（ES2022）
- **构建工具**：Vite
- **版本控制**：Git + GitHub
- **项目管理**：本地迭代记录（iterations/ 目录）

### 3D 正式版（规划中）
- **引擎/框架**：Unreal Engine 5
- **开发语言**：蓝图为主，C++ 做核心模块
- **建模**：Blender / Maya
- **贴图**：Substance Painter / Designer

## 当前重点

- [x] 搭建 Asitch 引擎骨架（Engine / Scene / Sprite / AssetManager）
- [ ] 实现角色移动、摄像机跟随
- [ ] 实现资源加载与场景切换
- [ ] 搭建 UE5 工程，验证 3D 版本可行性

## 约束与偏好

- 代码和命名使用英文
- 沟通使用中文
- 先做原型再优化
- 不引入重型依赖，引擎保持轻量
- 资源文件路径使用相对路径，方便迁移到 UE5

## 目录结构

```
asitch/
├── src/                    # 引擎核心源码
│   ├── engine.js           # Engine 主类
│   ├── scene.js            # Scene 场景管理
│   ├── sprite.js           # Sprite 精灵
│   ├── camera.js           # Camera 摄像机
│   ├── asset-manager.js    # AssetManager 资源管理
│   ├── input.js            # Input 输入系统
│   └── index.js            # 统一导出
├── examples/
│   └── minimal-web/        # 最小可运行示例
├── prompts/                # 提示词模板
├── configs/                # 项目上下文与 AI 设置
├── outputs/                # AI 生成的中间产物
├── iterations/             # 迭代记录
└── references/             # 参考资料、链接
```
