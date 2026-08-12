import { defineConfig } from 'vite';

export default defineConfig({
  root: '.',
  base: './',
  server: {
    port: 5173,
    open: 'examples/minimal-web/index.html',
  },
  build: {
    outDir: 'dist',
    emptyOutDir: true,
  },
});
