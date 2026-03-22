class ImageModal {
    constructor() {
        this.modal = document.getElementById("image-modal");
        this.modalImage = document.getElementById("image-modal-image");
        this.modalTitle = document.getElementById("image-modal-title");
        this.closeButton = document.getElementById("image-modal-close");
        this.figureButtons = document.querySelectorAll(".figure-button");
    }

    initialize() {
        for (let i = 0; i < this.figureButtons.length; i++) {
            this.figureButtons[i].addEventListener("click", this.handleFigureClick.bind(this));
        }

        this.closeButton.addEventListener("click", this.close.bind(this));
        this.modal.addEventListener("click", this.handleBackdropClick.bind(this));
        document.addEventListener("keydown", this.handleKeyDown.bind(this));
    }

    handleFigureClick(event) {
        const button = event.currentTarget;
        const imagePath = button.getAttribute("data-full-image");
        const title = button.getAttribute("data-title");
        const image = button.querySelector("img");

        this.modalImage.setAttribute("src", imagePath);
        this.modalImage.setAttribute("alt", image.getAttribute("alt"));
        this.modalTitle.textContent = title;
        this.modal.showModal();
    }

    handleBackdropClick(event) {
        const dialogBox = this.modal.getBoundingClientRect();
        const clickedOutside =
            event.clientX < dialogBox.left ||
            event.clientX > dialogBox.right ||
            event.clientY < dialogBox.top ||
            event.clientY > dialogBox.bottom;

        if (clickedOutside) {
            this.close();
        }
    }

    handleKeyDown(event) {
        if (event.key === "Escape" && this.modal.open) {
            this.close();
        }
    }

    close() {
        this.modal.close();
    }
}
