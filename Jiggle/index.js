const fs = require('fs');

const LI_ELEMENT = '<li class="list-group-item justify-content-between align-items-center list-group-item-secondary">';
const queue = [];

module.exports = async function (context, req) {

    context.log('JavaScript HTTP trigger function processed a request.');

    const action = extractCommand(req);

    if (isKnownUiAction(action)) {
        // known UI actions
        context.log(`Received request for action [${JSON.stringify(action)}]`);
        queue.push({
            action: action,
            dt: Date().toString()
        })
    }

    if (action === 'pop') {
        const item = queue.shift();
        context.res = {
            body: item && item.action
        };
        context.log(`Returning queued item [${item}]`)

    } else {
        // no known action, return UI.
        const buffer = fs.readFileSync("jiggle/ui.html");
        const commandList = LI_ELEMENT + queue.map(e => formatListItem(e)).join('</li>\n' + LI_ELEMENT) + '</li>';
        const html = buffer.toString().replace('~.~', commandList);
        //FORM + "<hr/>" + JSON.stringify(queue);

        context.res = {
            body: html,
            headers: {
                'Content-Type': 'text/html'
            }
        };
    }

};

function formatListItem(listItem) {
    return `<span class="badge badge-success">${listItem.action}</span> <p>${listItem.dt}</p>`
}

function extractCommand(req) {
    if (req.query && req.query.pop) {
        return 'pop';
    }

    if (isKnownUiAction(req.body)) {
        return req.body;
    }

    return undefined;
}

function isKnownUiAction(action) {
    return action && (action.startsWith('jiggle=') || action.startsWith('play='));
}