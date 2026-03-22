class ExperimentRepository {
    constructor(experiments) {
        this.experiments = experiments;
    }

    getAll() {
        return this.experiments;
    }

    getLabels() {
        const labels = [];

        for (let i = 0; i < this.experiments.length; i++) {
            labels.push(this.experiments[i].label);
        }

        return labels;
    }

    getValues(key) {
        const values = [];

        for (let i = 0; i < this.experiments.length; i++) {
            values.push(this.experiments[i][key]);
        }

        return values;
    }

    getMaxValue(key) {
        let maxValue = this.experiments[0][key];

        for (let i = 1; i < this.experiments.length; i++) {
            if (this.experiments[i][key] > maxValue) {
                maxValue = this.experiments[i][key];
            }
        }

        return maxValue;
    }
}
