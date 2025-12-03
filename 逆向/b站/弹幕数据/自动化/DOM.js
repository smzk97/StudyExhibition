let element = document.querySelector('.bpx-player-row-dm-wrap')
let observerOptions = {
    childList : true,
    attributes : true,
    subtree : true,
    characterData : true
};

let get_data = (node)=>{
    return node.textContent ? node.textContent.trim() : "";
}

let callback = (mutationLists)=> {
    for(let mutation of mutationLists) {
        if(mutation.type === 'childList') {
            if(mutation.parentNode.className.includes('bpx-player-row-dm-wrap')) {
                mutation.addedNodes.forEach((node)=> {
                    if(node.nodeName === "DIV" && node.nodeType === 1){
                        let comment_data = get_data(node);
                        if(node.last_data !== comment_data && comment_data){
                            node.last_data = comment_data;
                        }
                    }
                    }
                )
            }
            continue;
        }
        if(mutation.)
    }
}
let observer = new MutationObserver(callback);
observer.observe(element,observerOptions)