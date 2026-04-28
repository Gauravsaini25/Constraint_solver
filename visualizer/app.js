// App controller
(function() {
  const canvas = document.getElementById('canvas');
  const renderer = new Renderer(canvas);
  const tabs = document.getElementById('scenarioTabs');
  const info = document.getElementById('infoPanel');
  const cPanel = document.getElementById('constraintPanel');
  const overlay = document.getElementById('overlay');
  const slider = document.getElementById('progressSlider');
  const speedSlider = document.getElementById('speedSlider');
  const btnPlay = document.getElementById('btnPlay');
  const btnReset = document.getElementById('btnReset');

  let current = 0;
  let t = 1;
  let animating = false;
  let animFrame = null;

  // Build tabs
  SCENARIOS.forEach((s, i) => {
    const btn = document.createElement('button');
    btn.textContent = s.name;
    btn.addEventListener('click', () => selectScenario(i));
    tabs.appendChild(btn);
  });

  function selectScenario(i) {
    current = i;
    t = 1;
    slider.value = 1000;
    stopAnimation();
    updateTabs();
    updateInfo();
    draw();
  }

  function updateTabs() {
    Array.from(tabs.children).forEach((btn, i) => {
      btn.classList.toggle('active', i === current);
    });
  }

  function updateInfo() {
    const s = SCENARIOS[current];
    const convClass = s.converged ? 'pass' : 'fail';
    const convText = s.converged ? 'YES ✓' : 'NO ✗';

    info.innerHTML = `
      <h3>Scenario Info</h3>
      <p style="color:#8892a8;margin-bottom:10px;font-size:12px;">${s.desc}</p>
      <div class="row"><span>Status</span><span class="val ${convClass}">${convText}</span></div>
      <div class="row"><span>Iterations</span><span class="val">${s.iterations}</span></div>
      <div class="row"><span>Final Error</span><span class="val">${s.error.toExponential(2)}</span></div>
      <div class="row"><span>Points</span><span class="val">${s.initial.length}</span></div>
      <div class="row"><span>Constraints</span><span class="val">${s.constraints.length}</span></div>
    `;

    const typeColors = {
      distance: '#3b82f6', horizontal: '#fbbf24', vertical: '#a78bfa',
      angle: '#fb923c', coincident: '#f87171', fixed: '#6ee7b7'
    };

    let cHTML = '<h3>Constraints</h3>';
    for (const c of s.constraints) {
      const col = typeColors[c.type] || '#8892a8';
      const label = c.type.charAt(0).toUpperCase() + c.type.slice(1);
      const pts = c.p.map(i => `P${i}`).join(', ');
      const extra = c.val ? ` = ${c.val}${c.type === 'angle' ? '°' : ''}` : '';
      cHTML += `<div style="display:flex;align-items:center;gap:6px;margin-bottom:4px;">
        <span style="width:8px;height:8px;border-radius:50%;background:${col};flex-shrink:0;"></span>
        <span style="font-size:12px;color:${col};font-family:var(--mono);">${label}(${pts})${extra}</span>
      </div>`;
    }
    cPanel.innerHTML = cHTML;
  }

  function draw() {
    renderer.render(SCENARIOS[current], t);
    overlay.textContent = `t = ${t.toFixed(3)}  |  iteration ≈ ${Math.round(t * SCENARIOS[current].iterations)}`;
  }

  // Slider
  slider.addEventListener('input', () => {
    t = parseInt(slider.value) / 1000;
    stopAnimation();
    draw();
  });

  // Play
  btnPlay.addEventListener('click', () => {
    if (animating) { stopAnimation(); return; }
    if (t >= 0.999) t = 0;
    animating = true;
    btnPlay.textContent = '⏸ Pause';
    animate();
  });

  function animate() {
    if (!animating) return;
    const speed = parseInt(speedSlider.value);
    t += 0.003 * speed;
    if (t >= 1) { t = 1; stopAnimation(); }
    slider.value = Math.round(t * 1000);
    draw();
    animFrame = requestAnimationFrame(animate);
  }

  function stopAnimation() {
    animating = false;
    btnPlay.textContent = '▶ Animate';
    if (animFrame) cancelAnimationFrame(animFrame);
  }

  // Reset
  btnReset.addEventListener('click', () => {
    t = 0;
    slider.value = 0;
    stopAnimation();
    draw();
  });

  // Init
  selectScenario(0);
})();
