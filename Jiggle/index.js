const fs = require('fs');

const FORM = '<form action="/api/Jiggle" method="get">' +
    '<input type="radio" name="action" value="on">Jiggle!<br>' +
    '<input type="radio" name="action" value="beep">Beep!<br>' +
    '<input type="submit" value="submit"/></form>';

const queue = [];

module.exports = async function (context, req) {

    context.log('JavaScript HTTP trigger function processed a request.');

    const action = req.query.action || (req.body && req.body.action);

    if (action) {
        context.log(`Received request for action [${action}]`);
        queue.push(String(action).toLowerCase())
    }

    if (req.query.pop) {
        const item = queue.shift();
        context.res = {
            body: item
        };
        context.log(`Returning queued item [${item}]`)
    }
    else {

        const html =   fs.readFileSync("jiggle/ui.html");
        //FORM + "<hr/>" + JSON.stringify(queue);
        
        context.res = {
            body: html + JSON.stringify(queue),
            headers: {
                'Content-Type': 'text/html'
            }
        };
    }
};