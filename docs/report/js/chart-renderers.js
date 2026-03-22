class CanvasBarChartRenderer {
    constructor(repository) {
        this.repository = repository;
    }

    render(canvasId, title, valueKey, unit, colors) {
        const canvas = document.getElementById(canvasId);

        if (!canvas) {
            return;
        }

        const context = canvas.getContext("2d");
        const width = canvas.width;
        const height = canvas.height;
        const padding = 44;
        const chartWidth = width - padding * 2;
        const chartHeight = height - padding * 2;
        const maxValue = this.repository.getMaxValue(valueKey);
        const experiments = this.repository.getAll();
        const barWidth = chartWidth / experiments.length - 28;

        context.clearRect(0, 0, width, height);
        context.fillStyle = "#ffffff";
        context.fillRect(0, 0, width, height);

        context.strokeStyle = "#cbd5e1";
        context.lineWidth = 1;
        context.beginPath();
        context.moveTo(padding, padding);
        context.lineTo(padding, height - padding);
        context.lineTo(width - padding, height - padding);
        context.stroke();

        context.fillStyle = "#14213d";
        context.font = "bold 14px Segoe UI";
        context.fillText(title, padding, 24);

        for (let i = 0; i < experiments.length; i++) {
            const value = experiments[i][valueKey];
            const normalizedHeight = (value / maxValue) * (chartHeight - 18);
            const x = padding + 18 + i * (barWidth + 28);
            const y = height - padding - normalizedHeight;

            context.fillStyle = colors[i % colors.length];
            context.fillRect(x, y, barWidth, normalizedHeight);

            context.fillStyle = "#14213d";
            context.font = "12px Segoe UI";
            context.fillText(experiments[i].label, x + barWidth / 2 - 4, height - padding + 18);
            context.fillText(value.toFixed(2) + unit, x - 4, y - 8);
        }
    }
}

class ChartJsBarChartRenderer {
    constructor(repository) {
        this.repository = repository;
    }

    isAvailable() {
        return typeof Chart !== "undefined";
    }

    static destroyIfPresent(canvas) {
        const existing = Chart.getChart(canvas);
        if (existing) {
            existing.destroy();
        }
    }

    computeValueRange(valueKey) {
        const values = this.repository.getValues(valueKey);
        let minV = values[0];
        let maxV = values[0];
        for (let i = 1; i < values.length; i++) {
            if (values[i] < minV) {
                minV = values[i];
            }
            if (values[i] > maxV) {
                maxV = values[i];
            }
        }
        return { minV, maxV, values };
    }

    render(canvasId, label, valueKey, colors, unit, options) {
        const canvas = document.getElementById(canvasId);

        if (!canvas || !this.isAvailable()) {
            return false;
        }

        ChartJsBarChartRenderer.destroyIfPresent(canvas);

        const opts = options || {};
        const beginAtZero = opts.beginAtZero === true;
        const { minV, maxV } = this.computeValueRange(valueKey);
        let suggestedMin = undefined;
        let suggestedMax = undefined;

        if (!beginAtZero) {
            const span = Math.max(maxV - minV, 1e-9);
            suggestedMin = minV - span * 0.12;
            suggestedMax = maxV + span * 0.08;
        }

        new Chart(canvas, {
            type: "bar",
            data: {
                labels: this.repository.getLabels(),
                datasets: [
                    {
                        label: label,
                        data: this.repository.getValues(valueKey),
                        backgroundColor: colors,
                        borderRadius: 8,
                        borderSkipped: false,
                        maxBarThickness: 72
                    }
                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                layout: {
                    padding: { top: 8, right: 8, bottom: 4, left: 4 }
                },
                plugins: {
                    legend: {
                        display: false
                    },
                    tooltip: {
                        callbacks: {
                            label: function (context) {
                                const v = context.raw;
                                return label + ": " + v.toFixed(2) + unit;
                            }
                        }
                    }
                },
                scales: {
                    x: {
                        grid: {
                            display: false
                        },
                        ticks: {
                            font: { size: 12, weight: "600" }
                        }
                    },
                    y: {
                        beginAtZero: beginAtZero,
                        suggestedMin: suggestedMin,
                        suggestedMax: suggestedMax,
                        ticks: {
                            callback: function (value) {
                                return value + unit;
                            }
                        },
                        grid: {
                            color: "rgba(148, 163, 184, 0.25)"
                        }
                    }
                }
            }
        });

        return true;
    }
}
