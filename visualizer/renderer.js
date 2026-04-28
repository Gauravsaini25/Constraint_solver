// Canvas renderer for constraint solver visualization
class Renderer {
  constructor(canvas) {
    this.canvas = canvas;
    this.ctx = canvas.getContext('2d');
    this.dpr = window.devicePixelRatio || 1;
    this._setupHiDPI();
  }

  _setupHiDPI() {
    const rect = this.canvas.getBoundingClientRect();
    this.canvas.width = rect.width * this.dpr;
    this.canvas.height = rect.height * this.dpr;
    this.ctx.scale(this.dpr, this.dpr);
    this.w = rect.width;
    this.h = rect.height;
  }

  // Convert world coords to screen coords
  _toScreen(pt, bounds) {
    const pad = 80;
    const sx = (this.w - pad * 2) / (bounds.maxX - bounds.minX || 1);
    const sy = (this.h - pad * 2) / (bounds.maxY - bounds.minY || 1);
    const scale = Math.min(sx, sy);
    const cx = this.w / 2, cy = this.h / 2;
    const mx = (bounds.minX + bounds.maxX) / 2;
    const my = (bounds.minY + bounds.maxY) / 2;
    return {
      x: cx + (pt.x - mx) * scale,
      y: cy - (pt.y - my) * scale   // flip Y
    };
  }

  _getBounds(points) {
    let minX = Infinity, minY = Infinity, maxX = -Infinity, maxY = -Infinity;
    for (const p of points) {
      minX = Math.min(minX, p.x); minY = Math.min(minY, p.y);
      maxX = Math.max(maxX, p.x); maxY = Math.max(maxY, p.y);
    }
    const pad = 1.5;
    return { minX: minX - pad, minY: minY - pad, maxX: maxX + pad, maxY: maxY + pad };
  }

  clear() {
    this.ctx.clearRect(0, 0, this.w, this.h);
    this._drawGrid();
  }

  _drawGrid() {
    const ctx = this.ctx;
    ctx.strokeStyle = '#1e2235';
    ctx.lineWidth = 0.5;
    const step = 40;
    for (let x = 0; x < this.w; x += step) {
      ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, this.h); ctx.stroke();
    }
    for (let y = 0; y < this.h; y += step) {
      ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(this.w, y); ctx.stroke();
    }
  }

  render(scenario, t) {
    this.clear();
    const ctx = this.ctx;
    const pts = scenario.initial.map((ip, i) => ({
      x: ip.x + (scenario.final[i].x - ip.x) * t,
      y: ip.y + (scenario.final[i].y - ip.y) * t,
      fixed: ip.fixed || false,
    }));

    const allPts = [...scenario.initial, ...scenario.final];
    const bounds = this._getBounds(allPts);

    // Draw ghost initial positions
    if (t > 0.01 && t < 0.99) {
      for (let i = 0; i < scenario.initial.length; i++) {
        const sp = this._toScreen(scenario.initial[i], bounds);
        ctx.beginPath();
        ctx.arc(sp.x, sp.y, 5, 0, Math.PI * 2);
        ctx.strokeStyle = '#ffffff15';
        ctx.lineWidth = 1;
        ctx.stroke();
      }
    }

    // Draw constraints (lines between points)
    for (const c of scenario.constraints) {
      if (c.p.length >= 2) {
        const pa = this._toScreen(pts[c.p[0]], bounds);
        const pb = this._toScreen(pts[c.p[1]], bounds);
        ctx.beginPath();
        ctx.moveTo(pa.x, pa.y);
        ctx.lineTo(pb.x, pb.y);

        if (c.type === 'distance') {
          ctx.strokeStyle = '#3b82f680';
          ctx.setLineDash([6, 4]);
        } else if (c.type === 'horizontal') {
          ctx.strokeStyle = '#fbbf2460';
          ctx.setLineDash([3, 3]);
        } else if (c.type === 'vertical') {
          ctx.strokeStyle = '#a78bfa60';
          ctx.setLineDash([3, 3]);
        } else if (c.type === 'coincident') {
          ctx.strokeStyle = '#f8717160';
          ctx.setLineDash([2, 4]);
        } else if (c.type === 'angle' && c.p.length >= 3) {
          const pc = this._toScreen(pts[c.p[2]], bounds);
          ctx.strokeStyle = '#fb923c60';
          ctx.setLineDash([4, 3]);
          ctx.lineWidth = 1.5;
          ctx.stroke();
          ctx.beginPath();
          ctx.moveTo(pb.x, pb.y);
          ctx.lineTo(pc.x, pc.y);
        } else {
          ctx.strokeStyle = '#6ee7b740';
          ctx.setLineDash([]);
        }
        ctx.lineWidth = 1.5;
        ctx.stroke();
        ctx.setLineDash([]);

        // Label distance constraints
        if (c.type === 'distance' && c.val) {
          const mx = (pa.x + pb.x) / 2;
          const my = (pa.y + pb.y) / 2;
          ctx.font = '11px "JetBrains Mono"';
          ctx.fillStyle = '#3b82f6';
          ctx.fillText(`d=${c.val}`, mx + 6, my - 6);
        }
      }
    }

    // Draw angle arc
    for (const c of scenario.constraints) {
      if (c.type === 'angle' && c.p.length >= 3) {
        const vertex = this._toScreen(pts[c.p[1]], bounds);
        const pA = this._toScreen(pts[c.p[0]], bounds);
        const pC = this._toScreen(pts[c.p[2]], bounds);
        const a1 = Math.atan2(-(pA.y - vertex.y), pA.x - vertex.x);
        const a2 = Math.atan2(-(pC.y - vertex.y), pC.x - vertex.x);
        ctx.beginPath();
        ctx.arc(vertex.x, vertex.y, 25, -a2, -a1);
        ctx.strokeStyle = '#fb923c80';
        ctx.lineWidth = 2;
        ctx.stroke();
        const aMid = (a1 + a2) / 2;
        ctx.font = '11px "JetBrains Mono"';
        ctx.fillStyle = '#fb923c';
        ctx.fillText(`${c.val}°`, vertex.x + 30 * Math.cos(aMid), vertex.y - 30 * Math.sin(aMid));
      }
    }

    // Draw points
    for (let i = 0; i < pts.length; i++) {
      const sp = this._toScreen(pts[i], bounds);

      // Glow
      const grad = ctx.createRadialGradient(sp.x, sp.y, 0, sp.x, sp.y, 18);
      grad.addColorStop(0, pts[i].fixed ? '#f8717130' : '#6ee7b730');
      grad.addColorStop(1, 'transparent');
      ctx.fillStyle = grad;
      ctx.fillRect(sp.x - 18, sp.y - 18, 36, 36);

      // Point circle
      ctx.beginPath();
      ctx.arc(sp.x, sp.y, 6, 0, Math.PI * 2);
      if (pts[i].fixed) {
        ctx.fillStyle = '#f87171';
        ctx.fill();
        ctx.strokeStyle = '#fca5a5';
        ctx.lineWidth = 2;
        ctx.stroke();
        // Fixed icon (pin)
        ctx.beginPath();
        ctx.arc(sp.x, sp.y, 10, 0, Math.PI * 2);
        ctx.strokeStyle = '#f8717150';
        ctx.setLineDash([2, 2]);
        ctx.stroke();
        ctx.setLineDash([]);
      } else {
        ctx.fillStyle = '#6ee7b7';
        ctx.fill();
        ctx.strokeStyle = '#a7f3d0';
        ctx.lineWidth = 1.5;
        ctx.stroke();
      }

      // Label
      ctx.font = '600 12px "Inter"';
      ctx.fillStyle = '#e2e8f0';
      ctx.fillText(`P${i}`, sp.x + 12, sp.y - 10);

      // Coordinates
      ctx.font = '10px "JetBrains Mono"';
      ctx.fillStyle = '#8892a8';
      ctx.fillText(`(${pts[i].x.toFixed(2)}, ${pts[i].y.toFixed(2)})`, sp.x + 12, sp.y + 4);
    }
  }
}
