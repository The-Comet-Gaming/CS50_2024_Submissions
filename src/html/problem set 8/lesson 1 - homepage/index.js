document.getElementById("helloButton").addEventListener("click", function(event) {
    event.preventDefault();
    document.getElementById("helloAlert").style.display = "flex";
});
document.getElementById("closeButton").addEventListener("click", function(event) {
    event.preventDefault();
    document.getElementById("helloAlert").style.display = "none";
});
