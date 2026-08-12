/**
 * Asitch 2D engine — Sprite (entity base)
 * 轻量级 2D 精灵基类，支持图片渲染和纯色矩形回退。
 */
export class Sprite {
  /**
   * @param {object} [options]
   * @param {number} [options.x=0]
   * @param {number} [options.y=0]
   * @param {number} [options.width=64]
   * @param {number} [options.height=64]
   * @param {HTMLImageElement|null} [options.image=null]
   * @param {string} [options.color='#ffcc00']
   * @param {boolean} [options.visible=true]
   * @param {string} [options.tag='']
   */
  constructor(options = {}) {
    this.x = options.x ?? 0;
    this.y = options.y ?? 0;
    this.width = options.width ?? 64;
    this.height = options.height ?? 64;
    this.image = options.image ?? null;
    this.color = options.color ?? '#ffcc00';
    this.visible = options.visible ?? true;
    this.tag = options.tag ?? '';

    /** @type {import('./scene.js').Scene|null} 场景反向引用，由 Scene.add 设置 */
    this.scene = null;
  }

  /**
   * 设置精灵位置。
   * @param {number} x
   * @param {number} y
   * @returns {this}
   */
  setPosition(x, y) {
    this.x = x;
    this.y = y;
    return this;
  }

  /**
   * 设置精灵尺寸。
   * @param {number} w
   * @param {number} h
   * @returns {this}
   */
  setSize(w, h) {
    this.width = w;
    this.height = h;
    return this;
  }

  /**
   * 设置渲染所用图片。传入 null 则回退为纯色矩形。
   * @param {HTMLImageElement|null} img
   * @returns {this}
   */
  setImage(img) {
    this.image = img;
    return this;
  }

  /**
   * 返回当前 AABB 包围盒。
   * @returns {{ x: number, y: number, width: number, height: number }}
   */
  getBounds() {
    return {
      x: this.x,
      y: this.y,
      width: this.width,
      height: this.height,
    };
  }

  /**
   * 每帧更新钩子。基类空实现，子类按需覆盖。
   * @param {number} dt 距上一帧的时间，单位秒
   */
  update(dt) {
    // no-op
  }

  /**
   * 将精灵绘制到给定 Canvas 上下文。
   * 关闭 visible 时跳过。已加载的 image 优先，否则用 color 填充矩形。
   * @param {CanvasRenderingContext2D} ctx
   */
  render(ctx) {
    if (!this.visible) return;

    if (this.image && this.image.complete && this.image.naturalWidth > 0) {
      ctx.drawImage(this.image, this.x, this.y, this.width, this.height);
    } else {
      ctx.fillStyle = this.color;
      ctx.fillRect(this.x, this.y, this.width, this.height);
    }
  }

  /**
   * 与另一个 Sprite 做 AABB 碰撞检测。
   * @param {Sprite} other
   * @returns {boolean}
   */
  intersects(other) {
    return (
      this.x < other.x + other.width &&
      this.x + this.width > other.x &&
      this.y < other.y + other.height &&
      this.y + this.height > other.y
    );
  }
}

export default Sprite;
