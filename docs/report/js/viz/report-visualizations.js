class ReportVisualizations {
    constructor(assetBaseUrl) {
        this.assetBaseUrl = assetBaseUrl || "assets/";
        this.progressChart = new OptimizationProgressChart("progress-chart");
        this.routeMapView = null;
        this.routeAnimator = null;
        this.solution = null;
    }

    normalizeBaseUrl(url) {
        if (!url.endsWith("/")) {
            return url + "/";
        }
        return url;
    }

    async fetchJson(path) {
        const base = this.normalizeBaseUrl(this.assetBaseUrl);
        const response = await fetch(base + path);
        if (!response.ok) {
            throw new Error("Failed to load " + path);
        }
        return response.json();
    }

    setMessage(elementId, text) {
        const el = document.getElementById(elementId);
        if (el) {
            el.textContent = text;
        }
    }

    async initializeProgress() {
        const statusId = "progress-chart-status";
        this.setMessage(statusId, "");

        try {
            const rows = await this.fetchJson("exp_a_annealing_progress.json");
            const ok = this.progressChart.renderFromRows(rows);
            if (!ok) {
                this.setMessage(statusId, "Chart.js did not initialize.");
            }
        } catch (e) {
            this.setMessage(statusId, "Could not load progress data. Serve the report over HTTP or run python tools/generate_report_assets.py.");
        }
    }

    wireRouteControls() {
        const playBtn = document.getElementById("route-animation-play");
        const stopBtn = document.getElementById("route-animation-stop");
        if (!playBtn || !stopBtn || !this.routeAnimator) {
            return;
        }

        playBtn.addEventListener("click", () => {
            this.routeAnimator.start();
        });

        stopBtn.addEventListener("click", () => {
            this.routeAnimator.stop();
            this.routeAnimator.drawFrame(performance.now());
        });
    }

    async initializeRoutes() {
        const mapCanvas = document.getElementById("route-map-canvas");
        const animCanvas = document.getElementById("route-animation-canvas");
        const statusId = "route-viz-status";
        this.setMessage(statusId, "");

        try {
            this.solution = await this.fetchJson("exp_a_solution.json");
        } catch (e) {
            this.setMessage(
                statusId,
                "Could not load route geometry. Run python tools/generate_report_assets.py or export solution.json from the solver."
            );
            return;
        }

        if (mapCanvas) {
            this.routeMapView = new RouteMapView(mapCanvas, this.solution, { padding: 48 });
            this.routeMapView.draw();
            window.addEventListener(
                "resize",
                () => {
                    if (this.routeMapView) {
                        this.routeMapView.onResize();
                    }
                },
                { passive: true }
            );
        }

        if (animCanvas) {
            this.routeAnimator = new RouteAnimationController(animCanvas, this.solution, { padding: 48, staggerMs: 260 });
            this.routeAnimator.drawFrame(performance.now());
            window.addEventListener(
                "resize",
                () => {
                    if (this.routeAnimator) {
                        this.routeAnimator.onResize();
                    }
                },
                { passive: true }
            );
            this.wireRouteControls();
        }
    }

    async initialize() {
        await this.initializeProgress();
        await this.initializeRoutes();
    }

    getSolution() {
        return this.solution;
    }
}
