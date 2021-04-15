(function()
{
	var x = document.createElement("P");
	x.innerText="Welcome again";
	var btn=document.getElementById('LoginBtn');
	
	
	var loggedinChecker=new XMLHttpRequest();
	const loggedin="http://localhost:8000/?loggedin=1";
	loggedinChecker.open("GET",loggedin,true);
	loggedinChecker.send();
	loggedinChecker.onreadystatechange = (e) => {
		if (loggedinChecker.responseText == "yes")
		{
			document.body.innerHTML="TwoKey";
			document.body.appendChild(x);
			return;
		}

	}
	

	btn.addEventListener('click',f);
	function f()
	{
		var masterpasswd=document.getElementById('masterpasswd').value;
		var email=document.getElementById('email').value;
		
		var http=new XMLHttpRequest();
		const url="http://localhost:8000/?masterpasswd="+masterpasswd+"&email=" + email;
		http.open("GET",url,true);
		http.send();
		http.onreadystatechange = (e) => {
		console.log(http.readyState);
		if (http.readyState == 4 && http.status == 200) {
			document.body.innerHTML="TwoKey";
			document.body.appendChild(x);
		}
		else if (http.readyState == 4)
		{
			alert("wrong password");
		}
		console.log(http.responseText);
	  }

	}
})();