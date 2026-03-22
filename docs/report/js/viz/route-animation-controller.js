class RouteAnimationController {
    constructor(canvas, solution, options) {
        this.canvas = canvas;
        this.solution = solution;
        this.padding = options && options.padding !== undefined ? options.padding : 48;
        this.running = false;
        this.frameId = null;
        this.t0 = 0;
        this.routePolylines = [];
        this.routeLengths = [];
        this.routeDurations = [];
        this.totalCycleMs = 0;
        this.staggerMs = options && options.staggerMs !== undefined ? options.staggerMs : 280;
        this.onResize = this.onResize.bind(this);
        this.lastCssWidth = 0;
        this.lastCssHeight = 0;
    }

    buildGeometry(cssWidth, cssHeight) {
        const mapper = new CoordinateMapper(this.solution.nodes, cssWidth, cssHeight, this.padding);
        this.routePolylines = [];
        this.routeLengths = [];
        this.routeDurations = [];

        const routes = this.solution.routes;
        let sumDuration = 0;

        for (let r = 0; r < routes.length; r++) {
            const seq = routes[r];
            const pts = [];
            if (!seq || seq.length < 2) {
                this.routePolylines.push(pts);
                this.routeLengths.push([]);
                this.routeDurations.push(0);
                continue;
            }

            for (let i = 0; i < seq.length; i++) {
                pts.push(mapper.toCanvas(seq[i]));
            }

            const segLen = [];
            let total = 0;
            for (let i = 1; i < pts.length; i++) {
                const dx = pts[i].x - pts[i - 1].x;
                const dy = pts[i].y - pts[i - 1].y;
                const len = Math.hypot(dx, dy);
                segLen.push(len);
                total += len;
            }

            const durationMs = 2200 + Math.min(9000, total * 4.2);
            this.routePolylines.push(pts);
            this.routeLengths.push(segLen);
            this.routeDurations.push(durationMs);
            sumDuration = Math.max(sumDuration, this.staggerMs * r + durationMs);
        }

        this.totalCycleMs = sumDuration + 800;
    }

    positionOnRoute(pts, segLens, t) {
        if (!pts.length) {
            return null;
        }

        if (pts.length === 1) {
            return pts[0];
        }

        const total = segLens.reduce((a, b) => a + b, 0);
        if (total <= 0) {
            return pts[0];
        }

        let dist = t * total;
        for (let i = 0; i < segLens.length; i++) {
            if (dist <= segLens[i]) {
                const ratio = segLens[i] > 0 ? dist / segLens[i] : 0;
                const a = pts[i];
                const b = pts[i + 1];
                return {
                    x: a.x + (b.x - a.x) * ratio,
                    y: a.y + (b.y - a.y) * ratio
                };
            }
            dist -= segLens[i];
        }

        return pts[pts.length - 1];
    }

    onResize() {
        if (this.running) {
            this.drawFrame(performance.now());
        }
    }

    drawFrame(now) {
        const ctx = this.canvas.getContext("2d");
        const parent = this.canvas.parentElement;
        const cssWidth = parent ? parent.clientWidth : this.canvas.clientWidth || 800;
        const cssHeight = Math.max(360, Math.round(cssWidth * 0.92));

        const dpr = window.devicePixelRatio || 1;
        this.canvas.style.width = cssWidth + "px";
        this.canvas.style.height = cssHeight + "px";
        this.canvas.width = Math.floor(cssWidth * dpr);
        this.canvas.height = Math.floor(cssHeight * dpr);
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

        if (this.routePolylines.length === 0 || cssWidth !== this.lastCssWidth || cssHeight !== this.lastCssHeight) {
            this.buildGeometry(cssWidth, cssHeight);
            this.lastCssWidth = cssWidth;
            this.lastCssHeight = cssHeight;
        }

        const w = cssWidth;
        const h = cssHeight;
        const mapper = new CoordinateMapper(this.solution.nodes, w, h, this.padding);
        ctx.clearRect(0, 0, w, h);
        ctx.fillStyle = "#ffffff";
        ctx.fillRect(0, 0, w, h);

        const routes = this.solution.routes;
        for (let r = 0; r < routes.length; r++) {
            const pts = this.routePolylines[r];
            if (!pts || pts.length < 2) {
                continue;
            }

            const color = RoutePalette.colorAt(r);
            ctx.strokeStyle = color;
            ctx.lineWidth = 2.6;
            ctx.globalAlpha = 0.86;
            ctx.beginPath();
            ctx.moveTo(pts[0].x, pts[0].y);
            for (let i = 1; i < pts.length; i++) {
                ctx.lineTo(pts[i].x, pts[i].y);
            }
            ctx.stroke();
        }

        ctx.globalAlpha = 1;

        for (let i = 0; i < this.solution.nodes.length; i++) {
            const node = this.solution.nodes[i];
            const p = mapper.toCanvas(node.id);
            if (node.id === 0) {
                ctx.fillStyle = "#111827";
                ctx.beginPath();
                ctx.arc(p.x, p.y, 8, 0, Math.PI * 2);
                ctx.fill();
                ctx.strokeStyle = "#94a3b8";
                ctx.lineWidth = 2;
                ctx.setLineDash([5, 5]);
                ctx.beginPath();
                ctx.arc(p.x, p.y, 16, 0, Math.PI * 2);
                ctx.stroke();
                ctx.setLineDash([]);
                ctx.fillStyle = "#111827";
                ctx.font = "14px Segoe UI, sans-serif";
                ctx.fillText("Depot", p.x + 12, p.y - 10);
            } else {
                ctx.fillStyle = "#475569";
                ctx.beginPath();
                ctx.arc(p.x, p.y, 3.2, 0, Math.PI * 2);
                ctx.fill();
            }
        }

        const cycle = this.totalCycleMs > 0 ? this.totalCycleMs : 8000;
        const local = (now - this.t0) % cycle;

        for (let r = 0; r < routes.length; r++) {
            const pts = this.routePolylines[r];
            const lens = this.routeLengths[r];
            const dur = this.routeDurations[r];
            if (!pts || pts.length < 2 || dur <= 0) {
                continue;
            }

            const start = r * this.staggerMs;
            const u = (local - start) / dur;
            if (u < 0 || u > 1) {
                continue;
            }

            const pos = this.positionOnRoute(pts, lens, u);
            if (!pos) {
                continue;
            }

            const color = RoutePalette.colorAt(r);
            ctx.fillStyle = color;
            ctx.strokeStyle = "#ffffff";
            ctx.lineWidth = 2.5;
            ctx.beginPath();
            ctx.arc(pos.x, pos.y, 7.5, 0, Math.PI * 2);
            ctx.fill();
            ctx.stroke();
        }

        if (this.running) {
            this.frameId = requestAnimationFrame(this.drawFrame.bind(this));
        }
    }

    start() {
        if (this.running) {
            return;
        }

        this.running = true;
        this.routePolylines = [];
        this.t0 = performance.now();
        this.frameId = requestAnimationFrame(this.drawFrame.bind(this));
    }

    stop() {
        this.running = false;
        if (this.frameId !== null) {
            cancelAnimationFrame(this.frameId);
            this.frameId = null;
        }
    }
}
