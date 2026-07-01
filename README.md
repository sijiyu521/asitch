# Asitch — 轻量 2D 游戏引擎（Node 构建指南）

> 本说明将 README 聚焦为 Node.js 环境下的构建与运行指南；如果你的项目使用 Electron、Webpack、Vite 或其它工具，我也可以把说明细化到对应工具。

简短说明：Asitch 是一个轻量级 2D 游戏引擎模板，适合在浏览器或基于 Node 的打包工具下进行开发与原型制作。下面给出 Node（npm/yarn）为主的安装、构建与运行步骤，以及最小示例和推荐的 package.json 脚本。

---

## 前置要求

- Node.js（建议 16+ 或 LTS 版本）
- npm（随 Node 一起安装）或 yarn
- 建议安装全局静态服务器（可选）：`npm i -g serve`（用于本地预览 build 输出）

---

## 安装（第一次）

在仓库根目录运行：

```bash
git clone https://github.com/sijiyu521/asitch.git
cd asitch
npm install
# 或使用 yarn
# yarn install
```

如果仓库在 examples/ 或子包中包含独立示例，请进入对应子目录并安装依赖：

```bash
cd examples/minimal-web
npm install
```

---

## 常用 npm 脚本（推荐）

下面是推荐放入 `package.json` 的 scripts，README 中会使用这些脚本示例：

```json
{
  "scripts": {
    "dev": "vite",            // 开发模式（热重载）
    "build": "vite build",    // 打包为静态文件
    "start": "serve dist -s"  // 预览打包后的静态站点（需全局 serve 或替换为任意静态服务器）
  }
}
```

说明：这里以 Vite 举例（简单易用），你也可以改为 webpack/parcel 或自定义构建流程。

---

## 快速运行（开发模式）

推荐开发流程（使用 Vite 示例）：

1. 安装开发依赖（只需一次）：

   npm install --save-dev vite

2. 在 package.json 中添加 `dev` 脚本后，启动开发服务器：

   npm run dev

3. 浏览器打开 http://localhost:5173（默认）即可查看示例。

---

## 打包与预览（生产构建）

1. 运行构建：

   npm run build

2. 使用静态服务器预览：

   npm run start

（或 `npx serve dist -s`，或把 dist 部署到任意静态主机）

---

## 最小可运行示例（浏览器端）

下面给出一个极简的示例结构和关键文件，适合放在 `examples/minimal-web/`：

- examples/minimal-web/index.html
- examples/minimal-web/main.js
- examples/minimal-web/package.json

index.html：

```html
<!doctype html>
<html>
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Asitch - Minimal</title>
  </head>
  <body>
    <canvas id="gameCanvas" width="800" height="600"></canvas>
    <script type="module" src="./main.js"></script>
  </body>
</html>
```

main.js（伪代码，替换为实际引擎 API）：

```js
import { Engine, Scene, Sprite } from '../src/engine.js' // 若是打包模块，请改为正确路径

const canvas = document.getElementById('gameCanvas')
const engine = new Engine({ canvas })
const scene = new Scene()

// 加载资源（假设引擎提供 loadTexture）
engine.assets.loadTexture('player', '/assets/player.png').then(() => {
  const sprite = new Sprite('player')
  sprite.setPosition(100, 200)
  scene.add(sprite)

  engine.run(scene)
})
```

示例 package.json（minimal-web）:

```json
{
  "name": "asitch-minimal-web",
  "version": "0.0.1",
  "type": "module",
  "scripts": {
    "dev": "vite",
    "build": "vite build",
    "start": "serve dist -s"
  },
  "dependencies": {}
}
```

将示例放到 `examples/minimal-web/` 后：

```bash
cd examples/minimal-web
npm install
npm run dev
# 在浏览器打开 http://localhost:5173
```

---

## 运行示例（如果仓库已包含 examples/）

如果仓库自带 `examples/` 目录，请按每个示例目录下的 README 或 package.json 脚本执行。常见命令：

```bash
cd examples/<example-name>
npm install
npm run dev      # 或 npm run start
```

---

## 开发建议

- 推荐使用 ES 模块（type: "module"）并使用 Vite/Parcel/webpack 打包，方便热重载与现代语法支持。  
- 示例资源请放在 `assets/` 或 `examples/<name>/assets/` 下，并在开发服务器中通过相对路径加载。  
- 把常用工具脚本（如构建、资源压缩、纹理图集打包）放到 `scripts/`，并在 README 中给出使用示例。

---

## 贡献 & 许可证

如需我把 README 更新为中英双语、添加徽章（CI、License、npm version）、或生成 `examples/minimal-web/` 的完整示例文件（包含 sprites 和 package.json），我可以直接在仓库中创建这些文件并提交（或者在新分支上发 PR）。请告诉我你希望我现在做的下一步：

- A: 直接在主分支创建 `examples/minimal-web/` 完整示例并提交
- B: 在新分支创建示例并发起 PR
- C: 只在 README 中保留构建说明（已完成）

作者: sijiyu521
