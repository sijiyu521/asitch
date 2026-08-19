import { Engine, Scene, Sprite } from '../../src/index.js';

const canvas = document.getElementById('gameCanvas');
const engine = new Engine({ canvas, background: '#222' });

const scene = new Scene();
scene.background = '#0b0b0b';

const player = new Sprite({
  x: 0,
  y: 260,
  width: 64,
  height: 64,
  color: '#ffcc00',
  tag: 'player',
});

scene.add(player);

engine.assets.loadImage('player', '/examples/minimal-web/assets/player.svg')
  .then((img) => {
    player.setImage(img);
  })
  .catch(() => {
    // fallback to color rect
  });

const speed = 200;

player.update = function (dt) {
  const input = engine.input;

  if (input.isKeyDown('ArrowRight') || input.isKeyDown('KeyD')) {
    this.x += speed * dt;
  }
  if (input.isKeyDown('ArrowLeft') || input.isKeyDown('KeyA')) {
    this.x -= speed * dt;
  }

  if (this.x > canvas.width) this.x = -this.width;
  if (this.x < -this.width) this.x = canvas.width;
};

const hud = {
  update() {},
  render(ctx) {
    ctx.fillStyle = '#fff';
    ctx.font = '14px sans-serif';
    ctx.fillText('Asitch engine demo — Arrow keys / A D to move', 10, 20);
  },
};
scene.add(hud);

engine.run(scene);
