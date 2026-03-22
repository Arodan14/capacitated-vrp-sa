class RouteMapView {
    constructor(canvas, solution, options) {
        this.canvas = canvas;
        this.solution = solution;
        this.padding = options && options.padding !== undefined ? options.padding : 48;
        this.onResize = this.onResize.bind(this);
    }

    onResize() {
        this.draw();
    }

    draw() {
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

        const w = cssWidth;
        const h = cssHeight;
        const mapper = new CoordinateMapper(this.solution.nodes, w, h, this.padding);

        ctx.clearRect(0, 0, w, h);
        ctx.fillStyle = "#ffffff";
        ctx.fillRect(0, 0, w, h);

        const routes = this.solution.routes;
        for (let r = 0; r < routes.length; r++) {
            const seq = routes[r];
            if (!seq || seq.length < 2) {
                continue;
            }

            const color = RoutePalette.colorAt(r);
            ctx.strokeStyle = color;
            ctx.lineWidth = 2.25;
            ctx.globalAlpha = 0.88;
            ctx.lineJoin = "round";
            ctx.lineCap = "round";
            ctx.beginPath();
            const start = mapper.toCanvas(seq[0]);
            ctx.moveTo(start.x, start.y);

            for (let i = 1; i < seq.length; i++) {
                const p = mapper.toCanvas(seq[i]);
                ctx.lineTo(p.x, p.y);
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
                ctx.arc(p.x, p.y, 7, 0, Math.PI * 2);
                ctx.fill();
                ctx.fillStyle = "#334155";
                ctx.font = "13px Segoe UI, sans-serif";
                ctx.fillText("Depot", p.x + 10, p.y - 8);
            } else {
                ctx.fillStyle = "#475569";
                ctx.beginPath();
                ctx.arc(p.x, p.y, 3.2, 0, Math.PI * 2);
                ctx.fill();
            }
        }
    }
}
