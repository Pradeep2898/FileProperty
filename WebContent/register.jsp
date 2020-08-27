<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
<meta charset="ISO-8859-1">
<meta name="google-signin-scope" content="profile email">
<meta name="google-signin-client_id" content="798634521721-67hmg5phttgclhiik5t6bpenudq024j1.apps.googleusercontent.com">
<script src="https://apis.google.com/js/platform.js" async defer></script>
<title>New User</title>
</head>
<body>
	<center>
    <h3>User SignUp</h3>
    <form action="signup" id="signup">
    <label for="name">Name:</label>
    <input name="name" size="40" id="name"/><br><br>
    <label for="email">Email:</label>
    <input name="email" size="30" id="email"/><br><br>
    <label for="pword">Password:</label>
    <input type="password" name="password" id="password" size="30" />
    <b><p style="color:red;">${message}</b></p>
    <br>
    <button type="submit">SignUp</button>
    </form>
    <h5>or</h5>
    <p>Use Google SignIn</p>
    <div class="g-signin2" data-onsuccess="onSignIn" data-theme="dark"></div><br><br>
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
		document.getElementById('name').disabled = true;
		document.getElementById('password').disabled = true;
      }
    </script>
    </center>
  </body>
</html>