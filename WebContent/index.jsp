<html lang="en">
  <head>
    <meta name="google-signin-scope" content="profile email">
    <meta name="google-signin-client_id" content="798634521721-67hmg5phttgclhiik5t6bpenudq024j1.apps.googleusercontent.com">
    <script src="https://apis.google.com/js/platform.js" async defer></script>
  </head>
  <body>
  	<center>
	<h2>This page is an example of Google Outh Authorization and JDBC</h2><br><br>
	<%-- <div id="profileinfo"></div><br><br> --%>
	<form action="login" method="POST">
       <label for="email">Email:</label>
       <input name="email" size="30" id="email"/>
       <br><br>
       <label for="password">Password:</label>
       <input type="password" name="password" size="30" id="password"/>
       <br><b><p style="color:red;">${message}</b></p>
       <br>           
       <button type="submit">Login</button>
     </form>
     <h5>or</h5>
     <p>New user. <a href="register.jsp">SignUp Here</a>.
    <div class="g-signin2" data-onsuccess="onSignIn" data-theme="dark" id="Gbutton"></div><br><br>
    <script>
        var profile;
      function onSignIn(googleUser) {
        // Useful data for your client-side scripts:
        profile = googleUser.getBasicProfile();
        console.log("ID: " + profile.getId()); // Don't send this directly to your server!
        console.log('Full Name: ' + profile.getName());
        console.log('Given Name: ' + profile.getGivenName());
        console.log('Family Name: ' + profile.getFamilyName());
        console.log("Image URL: " + profile.getImageUrl());
        console.log("Email: " + profile.getEmail());
        // The ID token you need to pass to your backend:
        var id_token = googleUser.getAuthResponse().id_token;
        console.log("ID Token: " + id_token);
		document.getElementById('email').value = profile.getEmail();
		document.getElementById('password').value = profile.getId();
		document.getElementById('email').disabled = true;
		document.getElementById('password').disabled = true;
		document.getElementById("signout").disabled = false;
		document.getElementById("push").disabled = false;
      }

	  function signOut() {
			var auth2 = gapi.auth2.getAuthInstance();
			auth2.signOut().then(function() {document.getElementById('profileinfo').innerHTML + "";
			});
            location.reload();
	  }
    </script>
	    <button type="button" id="signout" onclick="signOut()" disabled>SignIn without Google</button>
    <%--<button type="button" id="push" onclick="location.href = 'process.jsp'" disabled>Enter data</button>
        <button type="button" onclick="location.href = 'retrieve.jsp'">View Table</button> --%>
    </center>
  </body>
</html>
