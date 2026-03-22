class EnlargeVisualModal {
    constructor() {
        this.dialog = document.getElementById("viz-enlarge-dialog");
        this.titleEl = document.getElementById("viz-enlarge-title");
        this.bodyEl = document.getElementById("viz-enlarge-body");
        this.closeBtn = document.getElementById("viz-enlarge-close");
        this._routeMapResize = null;

        if (this.closeBtn) {
            this.closeBtn.addEventListener("click", () => this.close());
        }

        if (this.dialog) {
            this.dialog.addEventListener("click", (e) => {
                if (e.target === this.dialog) {
                    this.close();
                }
            });
            this.dialog.addEventListener("close", () => {
                if (this._routeMapResize) {
                    window.removeEventListener("resize", this._routeMapResize);
                    this._routeMapResize = null;
                }
                if (this.bodyEl) {
                    this.bodyEl.innerHTML = "";
                }
            });
        }
    }

    close() {
        if (this.dialog && this.dialog.open) {
            this.dialog.close();
        }
    }

    openWithTitle(title) {
        if (this.titleEl) {
            this.titleEl.textContent = title;
        }
        if (this.bodyEl) {
            this.bodyEl.innerHTML = "";
        }
    }

    openCanvasSnapshot(canvasId, title) {
        const canvas = document.getElementById(canvasId);
        if (!canvas || !this.dialog || !this.bodyEl) {
            return;
        }

        this.openWithTitle(title);
        let dataUrl;
        try {
            dataUrl = canvas.toDataURL("image/png");
        } catch (e) {
            this.bodyEl.innerHTML = "<p class=\"viz-enlarge-error\">Could not capture this canvas (browser security).</p>";
            this.dialog.showModal();
            return;
        }

        const img = document.createElement("img");
        img.className = "viz-enlarge-img";
        img.src = dataUrl;
        img.alt = title;
        this.bodyEl.appendChild(img);
        this.dialog.showModal();
    }

    openRouteMap(solution, title) {
        if (!solution || !this.dialog || !this.bodyEl) {
            return;
        }

        this.openWithTitle(title);
        const wrap = document.createElement("div");
        wrap.className = "viz-enlarge-canvas-wrap";
        const canvas = document.createElement("canvas");
        canvas.className = "viz-enlarge-route-canvas";
        canvas.setAttribute("aria-label", title);
        wrap.appendChild(canvas);
        this.bodyEl.appendChild(wrap);

        const view = new RouteMapView(canvas, solution, { padding: 56 });
        const resize = () => view.draw();
        this._routeMapResize = resize;
        window.addEventListener("resize", resize, { passive: true });

        this.dialog.showModal();
        requestAnimationFrame(() => {
            requestAnimationFrame(resize);
        });
    }
}

class ReportEnlargeBindings {
    static attach(modal, reportVisualizations) {
        if (!modal || !modal.dialog) {
            return;
        }

        const snapshotButtons = document.querySelectorAll("[data-enlarge-snapshot]");
        for (let i = 0; i < snapshotButtons.length; i++) {
            const btn = snapshotButtons[i];
            btn.addEventListener("click", () => {
                const canvasId = btn.getAttribute("data-enlarge-snapshot");
                const title = btn.getAttribute("data-enlarge-title") || "Figure";
                modal.openCanvasSnapshot(canvasId, title);
            });
        }

        const mapButtons = document.querySelectorAll("[data-enlarge-route-map]");
        for (let j = 0; j < mapButtons.length; j++) {
            const btn = mapButtons[j];
            btn.addEventListener("click", () => {
                const sol = reportVisualizations.getSolution();
                const title = btn.getAttribute("data-enlarge-title") || "Route map";
                if (!sol) {
                    modal.openWithTitle(title);
                    modal.bodyEl.innerHTML =
                        "<p class=\"viz-enlarge-error\">Route data is not loaded. Serve the report over HTTP and ensure assets/exp_a_solution.json exists.</p>";
                    modal.dialog.showModal();
                    return;
                }
                modal.openRouteMap(sol, title);
            });
        }
    }
}
