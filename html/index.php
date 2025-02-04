#!/usr/bin/php-cgi

<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='utf-8'>
    <meta content='IE=Edge,chrome=1' http-equiv='X-UA-Compatible'>
    <meta content='width=device-width,height=device-height,user-scalable=no,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0'
          name='viewport'>
    <link href='https://profile.intra.42.fr/assets/42_logo_black-684989d43d629b3c0ff6fd7e1157ee04db9bb7a73fba8ec4e01543d650a1c607.png'
          rel='icon' type='image/png'>
    <link href='https://profile.intra.42.fr/assets/42_logo_black-684989d43d629b3c0ff6fd7e1157ee04db9bb7a73fba8ec4e01543d650a1c607.png'
          rel='shortcut icon' type='image/png'>
    <title>Webserv Profile Home</title>
    <link rel="stylesheet" media="all"
          href="https://profile.intra.42.fr/assets/application-ca812958a171bc1333f9fa65e409b756f2e11e453f02e827a5c886ddcc8b83b5.css"/>
    <link rel="stylesheet" href="resources/style.css"/>
</head>
<body class=' '>
<div class='main-navbar'>
    <div class='main-navbar-left'>
        <div class='main-navbar-logo' data-turbolinks-scaffold>
            <a href="#"><img alt="42"
                             src="https://profile.intra.42.fr/assets/42_logo-7dfc9110a5319a308863b96bda33cea995046d1731cebb735e41b16255106c12.svg"/>
            </a>
        </div>
        <a class="search-mobile-button hidden-sidebars fal fa-search" href="#"></a>
        <form action='#' class='main-navbar-search hidden-xs' method='get'>
            <input autocomplete='off' autofocus class='search-input' name='query' placeholder='Your URL...'>
        </form>
    </div>
    <div class='main-navbar-user-nav' data-turbolinks-scaffold>
        <div class='user-actions hidden-xs'>
            <a class="user-action labelable" data-notification-drop="" href="#"><span class='fal fa-bell'></span>
                <span class='user-action-count labeled' data-counter-count='0'>0</span>
            </a>
        </div>
        <span class='dropdown'>
            <a data-toggle='dropdown' href='#'>
                <span data-login='Tac'>Tac</span>
                <div class='user-profile-picture visible-sidebars'
                     style='background-image: url(https://cdn.intra.42.fr/users/7ae1a90e32adda926770d44c7531b90f/palmi.png)'></div>
            </a>
            <ul aria-labelledby='User menu' class='dropdown-menu' role='menu'>
            <li>
            <a href="#"><span class='iconf-calendar'></span>
            Manage slots
            </a></li>
            <li>
            <a href="#"><span class='iconf-user'></span>
            View my profile
            </a></li>
            <li>
            <a href="#">Settings</a>
            </li>
            <li>
            <a rel="nofollow" data-method="delete" href="#"><span
                    class='iconf-power-off'></span>
            <b>Logout</b>
            </a></li>
            </ul>
        </span>
        <div class='nav-separator hidden-xs'></div>
    </div>
</div>

<div class='page'>
    <div class='page-sidebar left-main-container page-sidebar-fixed-left under-main-navbar'>
        <ul class='main-left-navbar'>
            <div class='_'>
                <li>
                    <a class="active" href="#"><span class='icon-user-2'></span>
                        <span class='visible-overlay'>
                        Profile
                        </span>
                    </a></li>
                <li>
                    <a class="inactive" href="#"><span class='icon-network-2-1'></span>
                        <span class='visible-overlay'>
                        Projets
                        </span>
                    </a></li>
                <li>
                    <a class="inactive" href="#"><span class='icon-movie-play-1'></span>
                        <span class='visible-overlay'>
                        E-learning
                        </span>
                    </a></li>
                <li>
                    <a class="inactive" href="#"><span class='icon-briefcase'></span>
                        <span class='visible-overlay'>
                        Companies
                        </span>
                    </a></li>
                <li>
                    <a class="inactive" href="#"><span class='icon-compass-2'></span>
                        <span class='visible-overlay'>
                        Meta
                        </span>
                    </a></li>
                <li>
                    <a class="inactive" href="#"><span class='icon-shopping-1'></span>
                        <span class='visible-overlay'>
                        Shop
                        </span>
                    </a></li>
            </div>
        </ul>
        <div class='main-sidebar-left'></div>
    </div>
    <div class='page-content page-content-fluid' data-turbolinks-scaffold>
        <div class='row'>
            <div class='align-top'>
                <div class='home-middle-td'>
                    <div class='container-item profile-item full-width'>
                        <div class='container-inner-item profile-item-top profile-banner home-banner flex flex-direction-row'
                             style='background-image: url(https://cdn.intra.42.fr/coalition/cover/117/bg_classic_harkonnen.jpg)'>
                            <div class='user-banner margin-left-38 margin-right-10 visible-lg hidden-md'
                                 data-coalitions='2'
                                 data-default-url='https://profile.intra.42.fr/assets/coalitions/factionless-05b8cd65bda8f5eaf56ecf1d16493f41908801cfd66aa97fb27c9611064f4f36.svg'>
                                <a href="#">
                                    <div class='coalition-flag'>
                                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
                                        "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
                                        <svg xmlns="http://www.w3.org/2000/svg"
                                             xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" id="banner"
                                             x="0px" y="0px" viewBox="0 0 68 104"
                                             style="enable-background:new 0 0 68 104; fill: #c6c6c6;fill: #ffffff"
                                             xml:space="preserve" class="coalition-flag--flag">
                                            <g id="banner-content">
                                                <g id="UI-Intranet-banner-content"
                                                   transform="translate(-96.000000, -60.000000)">
                                                    <g id="banner-content-g-1"
                                                       transform="translate(96.000000, 60.000000)">
                                                        <polygon id="banner-content-polygon-1"
                                                                 points="0,0 0,80.5 34.3,104 68,80.5 68,0"></polygon>
                                                    </g>
                                                </g>
                                            </g>
                                        </svg>
                                        <img class="coalition-flag--icon"
                                             src="https://yt3.googleusercontent.com/afKWFz-iRbcgSU0yR6GsQcCrGuCNGnhnMKKcldyyjz3XomfEbBuDhIs_Vub9Wirwjt09Wd4gRnw=s176-c-k-c0x00ffffff-no-rj-mo">
                                    </div>
                                </a>
                            </div>
                            <div class='user-column flex flex-direction-column'>
                                <div class='user-primary' data-turbolinks-scaffold='inside'>
                                    <div class='col-sm-12 padding-left-30 user-infos'>
                                        <h2 class='profile-name margin-top-0 margin-bottom-0 flex flex-row align-items-center'>
                                            <div class='mini-banner visible-xs visible-sm visible-md inline margin-right-10 hidden-lg'>
                                                <a class="false"
                                                   href="#">
                                                    <div class='coalition-flag-mini'>
                                                        <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
                                                        "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
                                                        <svg xmlns="http://www.w3.org/2000/svg"
                                                             xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1"
                                                             id="banner" x="0px" y="0px" viewBox="0 0 68 104"
                                                             style="enable-background:new 0 0 68 104; fill: #c6c6c6;fill: #ffffff"
                                                             xml:space="preserve" class="coalition-flag--flag">
                                                            <g id="banner-content">
                                                                <g id="UI-Intranet-banner-content"
                                                                   transform="translate(-96.000000, -60.000000)">
                                                                    <g id="banner-content-g-1"
                                                                       transform="translate(96.000000, 60.000000)">
                                                                        <polygon id="banner-content-polygon-1"
                                                                                 points="0,0 0,80.5 34.3,104 68,80.5 68,0"></polygon>
                                                                    </g>
                                                                </g>
                                                            </g>
                                                        </svg>
                                                        <img class="coalition-flag--icon"
                                                             src="https://yt3.googleusercontent.com/afKWFz-iRbcgSU0yR6GsQcCrGuCNGnhnMKKcldyyjz3XomfEbBuDhIs_Vub9Wirwjt09Wd4gRnw=s176-c-k-c0x00ffffff-no-rj-mo">
                                                    </div>
                                                </a>
                                            </div>
                                            <span class='coalition-name margin-right-10'>
                                                <a href="#">
                                                    <span class='coalition-span' style='color: #ffffff'>
                                                        Webserv
                                                    </span>
                                                </a>
                                            </span>
                                            <span class='name margin-top-15 margin-bottom-10'><span>Tac</span></span>
                                            <div class='btn-group margin-top-15 margin-bottom-10 margin-left-10'
                                                 id='title-selector'>
                                                <button class='btn btn-default dropdown-toggle' data-toggle='dropdown'>
                                                    <span class='login' data-login='Tac'>Cookie connnecté 🍪 Tac</span>
                                                </button>
                                                <ul class='dropdown-menu'>
                                                    <li class='active'>
                                                        <a data-remote="true" data-method="get" href="#">Cookie
                                                            connnecté 🍪 Tac</a>
                                                    </li>
                                                </ul>
                                            </div>
                                        </h2>
                                    </div>
                                </div>
                                <div class='_'>
                                    <div class='col-md-3 padding-0 profile-infos profile-right-box'>
                                        <div class='user-header-box infos'>
                                            <div class='user-infos-sub'>
                                                <div class='user-wallet user-inline-stat'>
                                                    <span class='coalition-span' style='color: #E36E13'>
                                                        Wallet
                                                    </span>
                                                    <span class='user-wallet-value'>666&nbsp;₳</span>
                                                </div>
                                                <div class='user-correction-point user-inline-stat'>
                                                    <span class='coalition-span' style='color: #4180DB'>Evaluation points</span>
                                                    <span class='user-correction-point-value d-flex align-items-center'>
                                                        <div class='d-flex'>42</div>
                                                        <div class='user-grade user-inline-stat' data-pool='42cursus'>
                                                            <span data-placement='top'
                                                                  data-title='Give some of your points to the pool.'
                                                                  data-toggle='tooltip'>
                                                                <a class="ml-2 correction-point-btn" form_class="inline"
                                                                   data-class="modal-v2"
                                                                   data-smart-modal=""
                                                                   data-url="/user_data/104832/correction_point?campus_id=41&amp;cursus_slug=42cursus"
                                                                   data-selector=".modal-content"
                                                                   data-title=".modal-header" href="#"><span
                                                                        class='iconf-recycle'></span>
                                                                </a>
                                                            </span>
                                                        </div>
                                                    </span>
                                                </div>
                                                <div class='user-grade user-inline-stat'>
                                                    <span class='coalition-span' style='color: #4180DB'>
                                                    Cursus
                                                    </span>
                                                    <span class='user-cursus'>
                                                        <select class='cursus-user-select' style='width: 104px'>
                                                           <option selected='selected'
                                                                   value='42cursus'>42cursus</option>
                                                        </select>
                                                    </span>
                                                </div>
                                                <div class='user-grade user-inline-stat' data-cursus='42cursus'>
                                                    <span class='coalition-span' style='color: #4180DB'>Grade</span>
                                                    <span class='user-grade-value'>God</span>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <div class='profile-left-box'>
                                        <div class='user-primary' data-turbolinks-scaffold='inside'>
                                            <div class='user-infos col-sm-12'>
                                                <div class='user-data'>
                                                    <div class='user-header-box location'>
                                                        <div class='user-poste-status'>
                                                            <span class='icon-circle text-success'></span>
                                                            Available
                                                        </div>
                                                        <div class='user-poste-infos'>
                                                            c5r1p1
                                                        </div>
                                                    </div>
                                                </div>
                                                <div class='user-progress'>
                                                    <div class='cursus-progress-item'>
                                                        <a class='progress-container' data-cursus='42cursus'
                                                           href='#'>
                                                            <div class='progress double'>
                                                                <div class='progress-bar' role='progressbar'
                                                                     style='width: 100%; background: #4180DB'></div>
                                                                <div class='on-progress'>level 42 - 100%</div>
                                                            </div>
                                                        </a>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>

                    </div>
                    <div class='container-fullsize full-width fixed-height'>
                        <div class='row'>
                            <div class='col-lg-4 col-md-6 col-xs-12 fixed-height'>
                                <div class='container-inner-item boxed' data-turbolinks-scaffold='inside'>
                                    <h4 class='profile-title'>
                                        UPLOADED FILES
                                        <span class='pull-right'>
                                        <form action="/" method="post" enctype="multipart/form-data">
                                              <label for="file" class="upload-files">
                                                  <span>Choisir un fichier</span>
                                                  <input class="btn" type="file" id="file" name="file">
                                              </label>
                                              <input class="btn btn-primary" type="submit" value="Upload File">
                                            </form>
                                        </span>
                                    </h4>

                                    <div class='overflowable-item'>
                                        <div class='event-list' id='events-list'>


                                            <?php
                                            $uploadPath = "upload_files";
                                            $url = "http://localhost:8085/" . $uploadPath;
                                            $htmlContent = file_get_contents($url);
                                            $regex = '/<a href="\/upload_files\/(.*?)">(.*?)<\/a>/';
                                            preg_match_all($regex, $htmlContent, $matches, PREG_SET_ORDER);
                                            $fileNames = [];
                                            foreach ($matches as $match) {
                                                if ($match[1] !== '.' && $match[1] !== '..') {
                                                    $fileNames[] = $match[1];
                                                }
                                            }
                                            $i = 0;
                                            foreach ($fileNames as $fileName) {
                                                $i++;
                                            ?>

                                            <div class='event-item' data-event-category='other'>
                                                <div class='event-overflow'>
                                                </div>
                                                <div class='event-left' style="color: var(--event-dark-association)">
                                                    <div class='date-day'><?= $i ?></div>
                                                </div>
                                                <div class='event-main'>
                                                    <div class='event-top'>
                                                        <b class='event-name'><?= $fileName ?></b>
                                                    </div>
                                                    <div class='event-bottom d-flex align-content-center flex-wrap'>
                                                        <div class='event-time event-meta-item'>
                                                            <span class='icon-calendar-1'></span>
                                                            <span>10:00 AM</span>
                                                        </div>
                                                        <div class='event-date event-meta-item'>
                                                            <span class='icon-clock'></span>
                                                            <span>about 3 minutes</span>
                                                        </div>
                                                        <div class='event-location'>
                                                            <span class='icon-location'></span>
                                                            <span>C5</span>
                                                        </div>
                                                    </div>
                                                    <span class='event-theme'></span>
                                                    <div class='event-metadata'>
                                                        <a class="event-button"
                                                           href=<?= '"' . $uploadPath . '/' . $fileName . '"' ?>
                                                           >See file</a>
                                                         <a class="event-button deleteFileBtn"
                                                         style="background-color: #E74C3C"
                                                         name=<?= '"' . $fileName . '"' ?>
                                                          >Delete</a>
                                                    </div>
                                                </div>
                                            </div>
                                            <?php } ?>


                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class='col-lg-4 col-md-6 col-xs-12 fixed-height'>
                                <div class='container-inner-item boxed' data-turbolinks-scaffold='inside'>
                                    <h4 class='profile-title'>
                                        HTTP CODES
                                        <span class='simple-link pull-right'>
                                            <a aria-expended='true' class='btn simple-link' data-toggle='collapse'
                                               href='#' role='button'>
                                            Hide &#x025BE;
                                            </a>
                                            <a class="btn simple-link" href="#">Feedbacks logs</a>
                                            <a class="btn simple-link" href="#"><span class='icon-calendar'></span>
                                            Manage slots
                                            </a>
                                        </span>
                                    </h4>
                                    <div class='overflowable-item h-100'>
                                        <div aria-expended='true' class='collapse in' id='collapseEvaluations'>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/redir/test_301/">Redirection
                                                    301</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Moved Permanently</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/redir/test_308/">Redirection
                                                    308</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Permanent Redirect</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/" id="triggerBadRequest">Error 400</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Bad Request</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/test-403-forbidden.html">Error
                                                    403</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Forbidden</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/notexist">Error 404</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Not Found</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/test-methods/get-disallowed">Error
                                                    405</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span>Method Not Allowed</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/cgi-bin/py/infinite_loop.py">Error
                                                    408</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span data-placement="left" data-toggle="tooltip"
                                                              force_relative="" title=""
                                                              data-original-title="21 December 2012">Request Time-out</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <form class="form-413" action="/test-413-limit.html" method="post">
                                                    <label class="label-413" for="textarea-413">Long text :</label>
                                                    <textarea id="textarea-413" class="textarea-413" rows="1"></textarea>
                                                    <input class="submit-413 btn btn-primary" type="submit" value="Error 413">
                                                </form>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/error_pages/418.html"
                                                                              style="color: deeppink">Error
                                                    418</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span data-placement="left" data-toggle="tooltip"
                                                              force_relative="" title=""
                                                              data-original-title="1 April 1998">I’m a teapot</span>
                                                    </div>
                                                </div>
                                            </div>
                                            <div class='event project-item reminder'>
                                                <div class='project-item-text'>
                                                    You have to review the <a href="/cgi-bin/py/invalid-cgi.py">Error
                                                    500</a>
                                                </div>
                                                <div class='project-item-actions'>
                                                    <div class='time'>
                                                        <span class='iconf-bell'></span>
                                                        <span data-placement="left" data-toggle="tooltip"
                                                              force_relative="" title=""
                                                              data-original-title="00:00:00 UTC on 1 January 1970, the Unix epoch">
                                                            Internal Server Error</span>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <div class='col-lg-4 col-md-6 col-xs-12 fixed-height'>
                                <div class='container-inner-item boxed' data-turbolinks-scaffold='inside'>
                                    <h4 class='profile-title'>
                                        Projects
                                    </h4>
                                    <div style='position: relative;'>
                                        <span class="marked-title"><a href="#">webserv</a></span>
                                        <span class="project-item-lighteable">30 minutes later</span>
                                        <div class="pull-right text-success icon-star-8"
                                             title="Received 3 outstandings">
                                            125
                                        </div>
                                        <br><br>
                                        <iframe width="430" height="256"
                                                src="https://www.youtube.com/embed/RHljpE7pZh8?si=HDBCL7hEqWsx-cVr"
                                                title="YouTube video player" frameborder="0"
                                                allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share"
                                                referrerpolicy="strict-origin-when-cross-origin"
                                                allowfullscreen>
                                        </iframe>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<script>
document.getElementById('triggerBadRequest').addEventListener('click', function(event) {
    event.preventDefault();

    fetch('/', {
        method: 'POST', // Adjust depending on what the server expects
        headers: {
            'Content-Type': '::::::::::::', // Intentionally malformed Content-Type
        },
        body: JSON.stringify({ some: "data" }), // Adjust based on server's expectations
    })
    .then(async response => {
//         if (!response.ok) {
//             If the server responds with a client error (4xx) or server error (5xx), throw an error
//             const errorMessage = await response.text(); // or .json() if JSON response
//             throw new Error(errorMessage);
//         }
        return response.text(); // Process successful response
    })
    .then(data => {
        console.log('Success:', data);
    })
    .catch(error => {
        console.error('Error:', error.message);
        document.getElementById('response').innerText = 'Bad Request: ' + error.message;
    });
});



document.querySelectorAll('.deleteFileBtn').forEach(button => {
    button.addEventListener('click', function() {
        var nameAttribute = button.getAttribute('name');
        console.log(`nameAttribute: ${nameAttribute}`);
        fetch(`/upload_files/${nameAttribute}`, {
            method: 'DELETE'
        })
        .then(response => {
            if (response.ok) {
                return response.text();
            }
            throw new Error('Request failed!');
        })
        .then(data => {
            console.log(data);
        })
        .catch(error => {
            console.error(error);
        });
    });
});
</script>
</body>
</html>
