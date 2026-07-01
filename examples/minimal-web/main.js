const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// 简单示例：加载一个占位图并在画布上水平移动
const player = new Image();
player.src = '/assets/player.svg';

let x = 0;
let last = performance.now();

function update(dt) {
  x += 100 * dt; // 每秒移动 100px
  if (x > canvas.width) x = -64;
}

function render() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  // 背景网格
  ctx.fillStyle = '#0b0b0b';
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  // 绘制示例玩家
  if (player.complete) {
    ctx.drawImage(player, x, 260, 64, 64);
  } else {
    ctx.fillStyle = '#ffcc00';
    ctx.fillRect(x, 260, 64, 64);
  }

  // HUD
  ctx.fillStyle = '#fff';
  ctx.font = '14px sans-serif';
  ctx.fillText('Asitch minimal example — moving sprite', 10, 20);
}

function loop(now) {
  const dt = (now - last) / 1000;
  last = now;
  update(dt);
  render();
  requestAnimationFrame(loop);
}

player.onload = () => {
  requestAnimationFrame(loop);
};

// 若图片无法加载也启动循环
setTimeout(() => {
  if (!player.complete) requestAnimationFrame(loop);
}, 200);
