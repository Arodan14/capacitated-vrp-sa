class ReportApp {
    constructor() {
        this.repository = new ExperimentRepository(experimentData);
        this.reportCharts = new ReportCharts(this.repository);
        this.reportVisualizations = new ReportVisualizations("assets/");
        this.enlargeModal = new EnlargeVisualModal();
    }

    initialize() {
        this.reportCharts.initialize();
        const viz = this.reportVisualizations;
        const modal = this.enlargeModal;
        viz.initialize()
            .then(function () {
                ReportEnlargeBindings.attach(modal, viz);
            })
            .catch(function () {
                ReportEnlargeBindings.attach(modal, viz);
            });
    }
}

const reportApp = new ReportApp();
reportApp.initialize();
