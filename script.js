let height = 0, width = 0;
let edges = [];
let doc = document.getElementById("space");

function printMesh(){
    let tableHTML = '<table>';
    let counter = 0;
    for (let i = 0; i < height; i++) {
        tableHTML += '<tr>';
            for (let j = 0; j < width; j++) {
                tableHTML += '<td id="' + counter + '">' + counter + '</td>';
                counter++;
            }
        tableHTML += '</tr>';
    }
    tableHTML += '</table>';
    doc.innerHTML += tableHTML;
}

function createBorders(){
    for (let i = 3; i < edges.length; i += 2){
        if (edges[i] - edges[i - 1] == 1){
            document.getElementById(edges[i - 1]).style.borderRightWidth = "0";
            document.getElementById(edges[i]).style.borderLeftWidth = "0";
        }
        else if (edges[i] - edges[i - 1] == width){
            document.getElementById(edges[i - 1]).style.borderBottomWidth = "0";
            document.getElementById(edges[i]).style.borderTopWidth = "0";
        }
        console.log("*");
    }
}

function visualizeLabyrinth(event){
    const file = event.target.files[0];
    const reader = new FileReader();
    reader.onload = (e) => {
        const fileContent = e.target.result;
        edges = fileContent.split("\n");
        edges = edges.map((x) => parseInt(x));
        console.log("Liczby z pliku: ", edges);
        height = edges[0];
        width = edges[1];
        printMesh();
        createBorders();
    }  
    reader.readAsText(file);
}