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

        // 匹配不断增加的节点，直到childElementCount不变
        if(mutation.type === 'childList')
        {
            mutation.addedNodes.forEach((node)=> {
                if (node.nodeName === "DIV" && node.nodeType === 1) {
                    let comment_data = get_data(node);
                    if (comment_data && node._lastSeenText !== comment_data) {
                        console.log(`${comment_data}`);
                        node._lastSeenText = comment_data;
                    }
                }
            }
            )
            // 跳过后续的属性变化和字符变化检测，避免重复检测
            continue;
        }

        // 跳过style属性变化
        if(mutation.type === 'attributes' && mutation.attributeName === 'style')
            continue;

        let targetNode = mutation.target;
        let current_data = get_data(targetNode);
        if(current_data && targetNode._lastSeenText !== current_data)
        {
            console.log(`${current_data}`);
            targetNode._lastSeenText = current_data;
        }
    }
}

let observer = new MutationObserver(callback);
observer.observe(element,observerOptions);