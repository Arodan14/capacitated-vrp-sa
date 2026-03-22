class CoordinateMapper {
    constructor(nodes, canvasWidth, canvasHeight, padding) {
        this.nodesById = new Map();
        for (let i = 0; i < nodes.length; i++) {
            this.nodesById.set(nodes[i].id, nodes[i]);
        }

        let minX = nodes[0].x;
        let maxX = nodes[0].x;
        let minY = nodes[0].y;
        let maxY = nodes[0].y;

        for (let i = 1; i < nodes.length; i++) {
            const n = nodes[i];
            if (n.x < minX) {
                minX = n.x;
            }
            if (n.x > maxX) {
                maxX = n.x;
            }
            if (n.y < minY) {
                minY = n.y;
            }
            if (n.y > maxY) {
                maxY = n.y;
            }
        }

        this.minX = minX;
        this.maxX = maxX;
        this.minY = minY;
        this.maxY = maxY;
        this.padding = padding;
        this.canvasWidth = canvasWidth;
        this.canvasHeight = canvasHeight;
        this.scaleX = (canvasWidth - 2 * padding) / (maxX - minX + 1);
        this.scaleY = (canvasHeight - 2 * padding) / (maxY - minY + 1);
    }

    toCanvas(nodeId) {
        const node = this.nodesById.get(nodeId);
        if (!node) {
            return { x: this.padding, y: this.padding };
        }

        const x = this.padding + (node.x - this.minX) * this.scaleX;
        const y = this.canvasHeight - this.padding - (node.y - this.minY) * this.scaleY;
        return { x, y };
    }
}
