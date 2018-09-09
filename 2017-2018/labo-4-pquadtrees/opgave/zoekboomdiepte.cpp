<!DOCTYPE html>
<html>

<head>
    <meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<meta name="generator" content="intranetr/0.10.0">
<title>404 Not Found | iii intranet</title>    <link rel="stylesheet" type="text/css" media="screen,handheld" href="/css/default.css?0.10.0">
    <link rel="stylesheet" type="text/css" media="print" href="/css/print.css?0.10.0">
    <link rel="stylesheet" type="text/css" media="screen,handheld" href="/css/sites/default.css">
    <script type="text/javascript" src="/js/cache/0d6197553acf19ea859c96f7caf7d8fa6c6519b8.js?0.10.0"></script>
<!--[if lt IE 8]>
<link rel="stylesheet" type="text/css" href="/css/ie.css">
<![endif]-->
    <script type="text/javascript">
    (function() {
        var month = 1;
        iii.semester = month == 1 ? 1
            : month <= 6 ? 2
            : month <= 9 ? 0 : 1;
        if (iii.semester > 0) {
            var hideOtherSemester = function() {
                var other = iii.semester == 1 ? 2 : 1;
                $('#breadcrumbs li.group-sem'
                        + other
                        + ':not(.group-sem'
                        + iii.semester + '):not(.active)')
                    .hide();
            };
            $(document)
                .ready(hideOtherSemester)
                .on("contentUpdate.iii", hideOtherSemester);
        }
    })();
    </script>
</head>

<body>
    <div id="header"><h1>
    <a href="/" title="Naar de thuispagina">
        intranet
    </a>
</h1>
</div>
<div id="main">
<div id="content" class="simple error">
        <h2>404 Not Found</h2>
    <div id="page-content">
        <p>Bestand niet gevonden</p>
        <div id="page-content-end"></div>
    </div>
    <div id="footer"><div id="tag">intranetr/0.10.0</div>
<div id="footer-end"></div>
</div>
</div>
</div>
</body>

</html>
