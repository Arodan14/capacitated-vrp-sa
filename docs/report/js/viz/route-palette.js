class RoutePalette {
    static get colors() {
        return [
            "#2563eb",
            "#dc2626",
            "#16a34a",
            "#ea580c",
            "#7c3aed",
            "#0891b2",
            "#db2777",
            "#65a30d",
            "#4f46e5",
            "#b45309"
        ];
    }

    static colorAt(index) {
        const palette = RoutePalette.colors;
        return palette[index % palette.length];
    }
}
