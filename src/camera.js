/**
 * Asitch 2D 引擎 — Camera 类
 *
 * 摄像机：负责世界坐标到屏幕坐标的变换（平移 + 缩放），
 * 并可选地平滑跟随某个目标对象。
 *
 * @example
 *   const cam = new Camera({ viewport: canvas, x: 0, y: 0, zoom: 1 })
 *   cam.follow(player)
 *   // 每帧：
 *   cam.update(dt)
 *   cam.apply(ctx)
 *   // ... 绘制世界对象 ...
 *   cam.reset(ctx)
 */
export class Camera {
  /**
   * @param {object} [options={}]
   * @param {HTMLCanvasElement|{width:number,height:number}} [options.viewport]
   * @param {number} [options.x=0]        摄像机世界坐标 X
   * @param {number} [options.y=0]        摄像机世界坐标 Y
   * @param {number} [options.zoom=1]     缩放倍率（>1 放大，<1 缩小）
   * @param {number} [options.lerp=0.1]   跟随平滑插值系数（0~1，越大跟随越快）
   */
  constructor(options = {}) {
    this.x = options.x ?? 0
    this.y = options.y ?? 0
    this.zoom = options.zoom ?? 1
    this.lerp = options.lerp ?? 0.1

    this.target = null

    const vp = options.viewport
    if (vp) {
      if (typeof vp.width === 'number' && typeof vp.height === 'number') {
        this.viewportWidth = vp.width
        this.viewportHeight = vp.height
        this._canvas = null
      } else if (vp.tagName === 'CANVAS') {
        this._canvas = vp
        this.viewportWidth = vp.width
        this.viewportHeight = vp.height
      } else {
        this.viewportWidth = 0
        this.viewportHeight = 0
        this._canvas = null
      }
    } else {
      this.viewportWidth = 0
      this.viewportHeight = 0
      this._canvas = null
    }
  }

  /**
   * 设置要跟随的目标对象。
   * @param {{x:number, y:number}|null} targetObj  任何拥有 `.x` / `.y` 属性的对象，传 null 取消跟随
   */
  follow(targetObj) {
    this.target = targetObj ?? null
  }

  /**
   * 每帧调用：如果设置了跟随目标，则用 lerp 平滑地让摄像机中心对齐目标。
   * @param {number} dt  帧间隔（秒）。当前实现不直接使用 dt，
   *                     但保留参数以便后续扩展为帧率无关的缓动。
   */
  update(dt) {
    if (!this.target) return
    this.x += (this.target.x - this.x) * this.lerp
    this.y += (this.target.y - this.y) * this.lerp
  }

  /**
   * 把摄像机变换应用到 2D 渲染上下文：
   *   先 translate(-x*zoom + vpW*zoom/2, -y*zoom + vpH*zoom/2)
   *   再 scale(zoom, zoom)
   *
   * 使目标世界点 (x, y) 被绘制到屏幕中心。
   *
   * @param {CanvasRenderingContext2D} ctx
   */
  apply(ctx) {
    const w = this.viewportWidth
    const h = this.viewportHeight
    const z = this.zoom
    ctx.translate(-this.x * z + w * z * 0.5, -this.y * z + h * z * 0.5)
    ctx.scale(z, z)
  }

  /**
   * 撤销 {@link Camera#apply} 施加的变换（反向平移 + 反向缩放）。
   *
   * @param {CanvasRenderingContext2D} ctx
   */
  reset(ctx) {
    const w = this.viewportWidth
    const h = this.viewportHeight
    const z = this.zoom
    ctx.scale(1 / z, 1 / z)
    ctx.translate(this.x * z - w * z * 0.5, this.y * z - h * z * 0.5)
  }

  /**
   * 设置缩放倍率。
   * @param {number} z
   */
  setZoom(z) {
    this.zoom = z
  }

  /**
   * 更新视口尺寸（canvas resize 时调用）。
   * @param {number} w
   * @param {number} h
   */
  resize(w, h) {
    this.viewportWidth = w
    this.viewportHeight = h
    if (this._canvas) {
      this._canvas.width = w
      this._canvas.height = h
    }
  }
}

export default Camera
