/**
 * Asitch 2D — Input
 * Lightweight keyboard / mouse input manager.
 */
export class Input {
	/**
	 * @param {EventTarget} [target=window] - Target to attach listeners to.
	 */
	constructor(target = window) {
		/** @type {Set<string>} */
		this.keysDown = new Set();
		/** @type {Set<number>} */
		this.mouseDown = new Set();
		/** @type {{x:number, y:number, worldX:number, worldY:number}} */
		this.mouse = { x: 0, y: 0, worldX: 0, worldY: 0 };
		/** @type {Set<string>} */
		this.justPressed = new Set();
		/** @type {Set<string>} */
		this.justReleased = new Set();

		this._target = target;

		this._onKeyDown = (e) => {
			if (!this.keysDown.has(e.code)) {
				this.justPressed.add(e.code);
			}
			this.keysDown.add(e.code);
		};
		this._onKeyUp = (e) => {
			this.keysDown.delete(e.code);
			this.justReleased.add(e.code);
		};
		this._onMouseDown = (e) => {
			this.mouseDown.add(e.button);
		};
		this._onMouseUp = (e) => {
			this.mouseDown.delete(e.button);
		};
		this._onPointerMove = (e) => {
			this.mouse.x = e.clientX;
			this.mouse.y = e.clientY;
		};

		if (target && typeof target.addEventListener === 'function') {
			target.addEventListener('keydown', this._onKeyDown);
			target.addEventListener('keyup', this._onKeyUp);
			target.addEventListener('mousedown', this._onMouseDown);
			target.addEventListener('mouseup', this._onMouseUp);
			target.addEventListener('pointermove', this._onPointerMove);
		}
	}

	/**
	 * Check if a key is currently held down.
	 * @param {string} code - KeyboardEvent.code, e.g. 'KeyW', 'Space'.
	 * @returns {boolean}
	 */
	isKeyDown(code) {
		return this.keysDown.has(code);
	}

	/**
	 * Check if a key was pressed this frame only.
	 * @param {string} code - KeyboardEvent.code.
	 * @returns {boolean}
	 */
	isKeyPressed(code) {
		return this.justPressed.has(code);
	}

	/**
	 * Check if a mouse button is held down.
	 * @param {number} [btn=0] - 0 = left, 1 = middle, 2 = right.
	 * @returns {boolean}
	 */
	isMouseDown(btn = 0) {
		return this.mouseDown.has(btn);
	}

	/**
	 * Get current mouse coordinates (screen space).
	 * @returns {{x:number, y:number, worldX:number, worldY:number}}
	 */
	getMouse() {
		return this.mouse;
	}

	/**
	 * Clear per-frame state (justPressed / justReleased).
	 * Call at the end of each engine frame.
	 */
	consumeFrame() {
		this.justPressed.clear();
		this.justReleased.clear();
	}

	/**
	 * Remove all event listeners.
	 */
	destroy() {
		const t = this._target;
		if (t && typeof t.removeEventListener === 'function') {
			t.removeEventListener('keydown', this._onKeyDown);
			t.removeEventListener('keyup', this._onKeyUp);
			t.removeEventListener('mousedown', this._onMouseDown);
			t.removeEventListener('mouseup', this._onMouseUp);
			t.removeEventListener('pointermove', this._onPointerMove);
		}
	}
}

export default Input;
