# examples/minimal-web

这是一个基于 Vite 的极简运行示例（使用浏览器 Canvas）：

快速运行：

```bash
cd examples/minimal-web
npm install
npm run dev
```

打开浏览器并访问 http://localhost:5173 查看示例。  

构建为静态文件并预览：

```bash
npm run build
npm run start
```

说明：
- 示例使用原生 Canvas API 来演示一个移动的精灵。  
- 示例资源位于 `examples/minimal-web/assets/`。  
- 若你希望示例改为从 `src/` 引擎模块加载（方便展示引擎 API），请告诉我我会把示例改为依赖仓库中的实现并更新示例代码。
