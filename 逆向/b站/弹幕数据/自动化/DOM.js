let element = document.querySelector('.bpx-player-row-dm-wrap');
let config = {
    childList:true,
    attributes:true,
    subtree:true,
};

let printedSet = new Set();

let callback = (mutationlist)=>{
    for (let mutation of mutationlist){
        if(mutation.type === 'childList'){
            mutation.addedNodes.forEach((node)=>{
                if (node.nodeName === "DIV" && node.nodeType === 1) {
                    if (printedSet.has(node.innerText))
                        return;
                    printedSet.add(node.innerText.trim());
                    console.log(node.innerText.trim());
                }
            })
        }
        if (mutation.type === 'attributes'){
            let targetNode = mutation.target;
            if (targetNode.nodeName === "DIV" && targetNode.nodeType === 1 ) {
                if (printedSet.has(targetNode.innerText))
                    return;
                printedSet.add(targetNode.innerText.trim());
                console.log(targetNode.innerText.trim());
            }
        }
    }
}

let observer = new MutationObserver(callback);
observer.observe(element, config);