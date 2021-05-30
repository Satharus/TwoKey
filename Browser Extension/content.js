document.addEventListener("input",get_cred);
function get_cred(input)
{
    if(input.type="email")
    console.log(input.target.value);
    if(input.type="password")
    console.log(input);
}

chrome.runtime.onMessage.addListener(fillCreds);
function fillCreds(request,sender,sendResponse)
{
	var credentials = JSON.parse(request);
	console.log(credentials.email + "  " + credentials.password);

  document.addEventListener("click",fill_Creds);
function fill_Creds(input)
{
	console.log(input);
    if(input.srcElement.type==="email" || input.srcElement.type==="text")
	{
		input.target.value=credentials.email;
	}
    if(input.srcElement.type==="pass" || input.srcElement.type==="password")
	{
		input.target.value=credentials.password;
	}
}
}
