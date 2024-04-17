## Contributing

Hi there! We're thrilled that you'd like to contribute to this project. Your help is essential for keeping it great.

## Issues and PRs

If you have suggestions for how this project could be improved, or want to report a bug, open an issue! We'd love all and any contributions. If you have questions, too, we'd love to hear them.

We'd also love PRs. If you're thinking of a large PR, we advise opening up an issue first to talk about it

> [!IMPORTANT]  
> - Create new branch for your changes. make your changes there!
> - Do NOT add PR's to **main** branch.
> - Add PR's ONLY to **develop** branch.
> - File names and branch names MUST include "-" if needed.
> - File names and branch names should ONLY include simple letters.

## Folder Structure

1. root
   - assets
     - js
       - > add section name and then dash then name as you want 'home-foo.js'
     - css
       - > add section name and then dash then name as you want 'home-foo.css'
     - media
       - home
       - post
       - > make your section folder here for media
   - header-footer
       - footer.html
       - header.html
   - HTML's

> [!NOTE]  
> Theme should be using ![#2b1055](https://placehold.co/5/2b1055/2b1055.png) `#2b1055` to ![#7597de](https://placehold.co/1/7597de/7597de.png) `#7597de` and it shades only.


## Basic Template

### HTML

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>

    <link rel="apple-touch-icon" sizes="180x180" href="assets/media/header/favicon/apple-touch-icon.png">
    <link rel="icon" type="image/png" sizes="32x32" href="assets/media/header/favicon/favicon-32x32.png">
    <link rel="icon" type="image/png" sizes="16x16" href="assets/media/header/favicon/favicon-16x16.png">
    <link rel="manifest" href="assets/media/header/favicon/site.webmanifest">

    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
    <link rel="stylesheet" href="assets/css/YOUR-CSS.css">
    <script src="assets/js/header-footer.js"></script>

</head>
<body>
    <div id="header"></div>

    <div class="sec">
        <h2><!-- YOUR SECTION TITLE GOES HERE --></h2>
        <hr>
            <!-- YOUR CODE GOES HERE -->
    </div>

    <div id="footer"></div>
</body>
</html>
```

### CSS

```css
@import url('https://fonts.googleapis.com/css?family=Poppins:300,400,500,600,700,800,900&display=swap');

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Poppins', sans-serif;
}

body {
    min-height: 100vh;
    background: linear-gradient(#2b1055, #7597de);
    overflow-x: hidden;
}

header {
    content: '';
    background: linear-gradient(to top, #1c0522, #4a488e);
    color: #fff;
}

/* section must include this */
.sec {
    position: relative;
    padding: 100px;
    background: #1c0522;
}

.sec h2{
    font-weight: 600;
    color: #fff;
    font-size: 40px;
    margin-bottom: 5px;
}
.sec hr {
    border: none;
    width: 120px;
    height: 5px;
    background-color: #fff;
    border-radius: 10px;
    margin-bottom: 20px;
}
```