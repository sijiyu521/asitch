import Scene from './scene.js';
import AssetManager from './asset-manager.js';
import Input from './input.js';
import Camera from './camera.js';

/**
 * Asitch Engine — lightweight 2D Canvas game engine.
 */
export default class Engine {
  /**
   * @param {object} [options]
   * @param {HTMLCanvasElement} [options.canvas] - Target canvas. Auto-created if omitted.
   * @param {number} [options.width]  - Canvas width. Defaults to canvas.clientWidth.
   * @param {number} [options.height] - Canvas height. Defaults to canvas.clientHeight.
   * @param {string} [options.background] - Clear color. Default '#222'.
   */
  constructor({ canvas, width, height, background = '#222' } = {}) {
    if (!canvas) {
      canvas = document.createElement('canvas');
      document.body.appendChild(canvas);
    }

    this.canvas = canvas;
    this.ctx = canvas.getContext('2d');
    this.width = width ?? canvas.clientWidth;
    this.height = height ?? canvas.clientHeight;
    this.background = background;

    canvas.width = this.width;
    canvas.height = this.height;

    this.running = false;
    this.currentScene = null;
    this.lastTime = 0;
    this.fixedDT = 0;

    this.assets = new AssetManager();
    this.input = new Input();
    this.camera = new Camera();

    this._rafId = null;
    this.loop = this.loop.bind(this);
  }

  /**
   * Start the engine with the given scene.
   * @param {Scene} scene
   */
  run(scene) {
    this.setScene(scene);
    if (this.running) return;
    this.running = true;
    this.lastTime = performance.now();
    this._rafId = requestAnimationFrame(this.loop);
  }

  /** Stop the engine loop. */
  stop() {
    this.running = false;
    if (this._rafId !== null) {
      cancelAnimationFrame(this._rafId);
      this._rafId = null;
    }
  }

  /**
   * Internal RAF loop. Calculates dt, updates and renders the current scene.
   * @param {number} now - Current timestamp from requestAnimationFrame.
   * @internal
   */
  loop(now) {
    if (!this.running) return;

    const rawDT = (now - this.lastTime) / 1000;
    this.lastTime = now;
    const dt = this.fixedDT > 0 ? this.fixedDT : rawDT;

    this.currentScene?.update(dt);

    this.ctx.fillStyle = this.background;
    this.ctx.fillRect(0, 0, this.width, this.height);
    this.currentScene?.render(this.ctx);

    this._rafId = requestAnimationFrame(this.loop);
  }

  /**
   * Transition to a new scene. Calls onExit on the old scene and onEnter on the new one.
   * @param {Scene} scene
   */
  setScene(scene) {
    if (this.currentScene && typeof this.currentScene.onExit === 'function') {
      this.currentScene.onExit();
    }
    this.currentScene = scene;
    if (scene && typeof scene.onEnter === 'function') {
      scene.onEnter(this);
    }
  }

  /**
   * Resize the engine canvas.
   * @param {number} w
   * @param {number} h
   */
  resize(w, h) {
    this.width = w;
    this.height = h;
    this.canvas.width = w;
    this.canvas.height = h;
  }
}
