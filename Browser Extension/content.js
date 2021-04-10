// (function () {
//     Array.prototype.forEach.call(document.querySelectorAll('form input[type=password]'), function (pass) {
//         var form = pass.closest('form');

//         // <input> and <input type=""> do not get picked up by input[type=text] selector
//         // so we must select all inputs and filter it with the .type property
//         var users = Array.prototype.filter.call(form.querySelectorAll('input'), function (input) {
//             return input.type == 'text' || input.type == 'email';
//         });
//         alert(users.value);
      
//     });
// })();
document.addEventListener("input",get_cred);
function get_cred(input)
{
    if(input.type="email")
    console.log(input.target.value);
    if(input.type="password")
    console.log(input);
}



