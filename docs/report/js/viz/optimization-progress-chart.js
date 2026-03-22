class OptimizationProgressChart {
    constructor(canvasId) {
        this.canvasId = canvasId;
        this.chart = null;
    }

    isAvailable() {
        return typeof Chart !== "undefined";
    }

    destroy() {
        const canvas = document.getElementById(this.canvasId);
        if (!canvas) {
            return;
        }

        const existing = Chart.getChart(canvas);
        if (existing) {
            existing.destroy();
        }

        this.chart = null;
    }

    renderFromRows(rows) {
        const canvas = document.getElementById(this.canvasId);
        if (!canvas || !this.isAvailable() || !rows || rows.length === 0) {
            return false;
        }

        this.destroy();

        const labels = [];
        const costs = [];
        const temps = [];

        for (let i = 0; i < rows.length; i++) {
            labels.push(rows[i].step);
            costs.push(rows[i].bestCost);
            temps.push(Math.max(rows[i].temperature, 1e-9));
        }

        this.chart = new Chart(canvas, {
            type: "line",
            data: {
                labels: labels,
                datasets: [
                    {
                        label: "Best cost",
                        data: costs,
                        yAxisID: "y",
                        borderColor: "#1d4ed8",
                        backgroundColor: "rgba(29, 78, 216, 0.12)",
                        fill: true,
                        tension: 0.12,
                        pointRadius: 0,
                        borderWidth: 2
                    },
                    {
                        label: "Temperature",
                        data: temps,
                        yAxisID: "y1",
                        borderColor: "#ea580c",
                        backgroundColor: "rgba(234, 88, 12, 0.06)",
                        fill: false,
                        tension: 0.12,
                        pointRadius: 0,
                        borderWidth: 1.6
                    }
                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                interaction: {
                    mode: "index",
                    intersect: false
                },
                plugins: {
                    legend: {
                        display: true,
                        position: "top"
                    },
                    tooltip: {
                        callbacks: {
                            label: function (context) {
                                const ds = context.dataset.label || "";
                                if (ds === "Temperature") {
                                    return "Temperature: " + context.raw.toExponential(3);
                                }
                                return "Best cost: " + context.raw.toFixed(2);
                            }
                        }
                    }
                },
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: "Iteration step (same temperature may repeat across inner iterations)"
                        },
                        ticks: {
                            maxTicksLimit: 12
                        }
                    },
                    y: {
                        type: "linear",
                        position: "left",
                        title: {
                            display: true,
                            text: "Best feasible cost"
                        },
                        ticks: {
                            callback: function (value) {
                                return value.toFixed(0);
                            }
                        }
                    },
                    y1: {
                        type: "logarithmic",
                        position: "right",
                        title: {
                            display: true,
                            text: "Temperature (log scale)"
                        },
                        grid: {
                            drawOnChartArea: false
                        },
                        ticks: {
                            callback: function (value) {
                                return value.toExponential(0);
                            }
                        }
                    }
                }
            }
        });

        return true;
    }
}
