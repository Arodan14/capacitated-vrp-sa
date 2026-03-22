class ReportCharts {
    constructor(repository) {
        this.repository = repository;
        this.chartJsRenderer = new ChartJsBarChartRenderer(repository);
        this.canvasRenderer = new CanvasBarChartRenderer(repository);
    }

    initialize() {
        const usedChartJs = this.chartJsRenderer.render("cost-chart", "Final cost", "cost", ["#2563eb", "#60a5fa", "#93c5fd"], "", {
            beginAtZero: false
        });

        if (usedChartJs) {
            this.chartJsRenderer.render("runtime-chart", "Runtime", "runtime", ["#ea580c", "#fb923c", "#fdba74"], " s", {
                beginAtZero: true
            });
            this.chartJsRenderer.render("distance-chart", "Distance", "distance", ["#16a34a", "#4ade80", "#86efac"], "", {
                beginAtZero: false
            });
            return;
        }

        this.canvasRenderer.render("cost-chart", "Final Cost by Experiment", "cost", "", ["#2563eb", "#60a5fa", "#93c5fd"]);
        this.canvasRenderer.render("runtime-chart", "Runtime by Experiment", "runtime", "s", ["#ea580c", "#fb923c", "#fdba74"]);
        this.canvasRenderer.render("distance-chart", "Distance by Experiment", "distance", "", ["#16a34a", "#4ade80", "#86efac"]);
    }
}
