/**
 * AssetManager — Asitch 2D 引擎的轻量资源管理器
 *
 * 负责加载、缓存、查询 images / audio / json 三类资源。
 * - 纯 ES module，无外部依赖
 * - 对同一 key 的重复 load 调用会返回首次加载的 Promise，避免重复请求
 * - 每种资源类型使用独立的 Map 缓存，结构为：{ data, promise }
 */

const TYPE_MAP = {
  image: 'images',
  audio: 'audio',
  json: 'json',
};

export class AssetManager {
  constructor() {
    this.images = new Map();
    this.audio = new Map();
    this.json = new Map();
  }

  /**
   * 内部工具：根据 type 返回对应的 Map。
   * @param {'image'|'audio'|'json'} type
   * @returns {Map}
   */
  _store(type) {
    const name = TYPE_MAP[type];
    if (!name) throw new Error(`AssetManager: unknown asset type "${type}"`);
    return this[name];
  }

  /**
   * 加载单张图片。
   * @param {string} key  资源键名
   * @param {string} src   图片地址
   * @returns {Promise<HTMLImageElement>}
   */
  loadImage(key, src) {
    return this._load(key, src, 'image', () => {
      const img = new Image();
      return new Promise((resolve, reject) => {
        img.onload = () => resolve(img);
        img.onerror = () => reject(new Error(`Failed to load image: ${src}`));
        img.src = src;
      });
    });
  }

  /**
   * 批量加载图片。
   * @param {Record<string, string>} sources  { key: src } 映射
   * @returns {Promise<Record<string, HTMLImageElement>>}
   */
  loadImages(sources) {
    const entries = Object.entries(sources);
    return Promise.all(entries.map(([k, s]) => this.loadImage(k, s))).then((imgs) =>
      entries.reduce((acc, [k], i) => {
        acc[k] = imgs[i];
        return acc;
      }, {})
    );
  }

  /**
   * 加载单个音频资源。
   * @param {string} key
   * @param {string} src
   * @returns {Promise<HTMLAudioElement>}
   */
  loadAudio(key, src) {
    return this._load(key, src, 'audio', () => {
      const audio = new Audio();
      return new Promise((resolve, reject) => {
        audio.addEventListener('canplaythrough', () => resolve(audio), { once: true });
        audio.addEventListener('error', () => reject(new Error(`Failed to load audio: ${src}`)), { once: true });
        audio.src = src;
        audio.load();
      });
    });
  }

  /**
   * 加载 JSON 资源。
   * @param {string} key
   * @param {string} src
   * @returns {Promise<object>}
   */
  loadJSON(key, src) {
    return this._load(key, src, 'json', () =>
      fetch(src).then((r) => {
        if (!r.ok) throw new Error(`Failed to load JSON: ${src} (${r.status})`);
        return r.json();
      })
    );
  }

  /**
   * 统一的加载实现：处理缓存和"正在加载"去重。
   * @param {string} key
   * @param {string} src
   * @param {'image'|'audio'|'json'} type
   * @param {() => Promise<any>} factory 真正创建资源 Promise 的工厂
   * @returns {Promise<any>}
   */
  _load(key, src, type, factory) {
    const store = this._store(type);
    const entry = store.get(key);

    if (entry) {
      if (entry.data !== undefined) return Promise.resolve(entry.data);
      return entry.promise;
    }

    const promise = factory()
      .then((data) => {
        store.set(key, { data, promise });
        return data;
      })
      .catch((err) => {
        store.delete(key);
        throw err;
      });

    store.set(key, { data: undefined, promise });
    return promise;
  }

  /**
   * 取已缓存的图片；未加载完成或不存在时返回 undefined。
   * @param {string} key
   * @returns {HTMLImageElement | undefined}
   */
  getImage(key) {
    return this.images.get(key)?.data;
  }

  /**
   * 取已缓存的音频。
   * @param {string} key
   * @returns {HTMLAudioElement | undefined}
   */
  getAudio(key) {
    return this.audio.get(key)?.data;
  }

  /**
   * 取已缓存的 JSON 对象。
   * @param {string} key
   * @returns {object | undefined}
   */
  getJSON(key) {
    return this.json.get(key)?.data;
  }

  /**
   * 判断指定 key 的资源是否已加载完成。
   * @param {string} key
   * @param {'image'|'audio'|'json'} [type='image']
   * @returns {boolean}
   */
  has(key, type = 'image') {
    return this._store(type).get(key)?.data !== undefined;
  }

  /** 清空所有缓存（包括正在加载中的）。 */
  clear() {
    this.images.clear();
    this.audio.clear();
    this.json.clear();
  }
}

export default AssetManager;
