chrome.runtime.onMessage.addListener(gotUrl);
function gotUrl(theUrl,sender,sendResponse)
{
    console.log(theUrl);
}


/*(function () {
    Array.prototype.forEach.call(document.querySelectorAll('form input[type=password]'), function (pass) {
        var form = pass.closest('form');

       
        var users = Array.prototype.filter.call(form.querySelectorAll('input'), function (input) {
            return input.type == 'text' || input.type == 'email';
        });*/

