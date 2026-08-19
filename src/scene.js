/**
 * Scene — Asitch 2D engine 的场景容器。
 *
 * 负责维护一组实体（entity），并在每一帧依次驱动它们的 update / render。
 * 同时持有相机引用与背景色，作为渲染的最底层画布。
 */
export class Scene {
  constructor() {
    /** @type {Array<{update(dt: number): void, render(ctx: CanvasRenderingContext2D): void}>} */
    this.entities = [];
    /** @type {import('./camera.js').Camera | null} */
    this.camera = null;
    /** @type {string} */
    this.background = '#111';
  }

  /**
   * 将实体加入场景，并把实体的 scene 反向引用指向当前场景。
   * @param {{update(dt: number): void, render(ctx: CanvasRenderingContext2D): void, scene?: Scene}} entity
   */
  add(entity) {
    this.entities.push(entity);
    entity.scene = this;
  }

  /**
   * 从场景中移除实体。
   * @param {{update(dt: number): void, render(ctx: CanvasRenderingContext2D): void}} entity
   */
  remove(entity) {
    const i = this.entities.indexOf(entity);
    if (i !== -1) this.entities.splice(i, 1);
  }

  /**
   * 逐帧更新所有实体。
   * @param {number} dt 距上一帧的秒数
   */
  update(dt) {
    for (const e of this.entities) e.update(dt);
  }

  /**
   * 先用背景色清空画布，再依次渲染每个实体。
   * @param {CanvasRenderingContext2D} ctx
   */
  render(ctx) {
    ctx.fillStyle = this.background;
    ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);
    for (const e of this.entities) e.render(ctx);
  }

  /**
   * 场景被引擎激活时调用的钩子，子类可覆盖以执行初始化。
   * @param {import('./engine.js').Engine} engine
   */
  onEnter(engine) {}

  /**
   * 场景被引擎切走时调用的钩子，子类可覆盖以执行清理。
   */
  onExit() {}
}

export default Scene;
