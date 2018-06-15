<?php
// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2014 University of California
//
// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.

// User agrees to the terms of use.
// Logs user in by sending a cookie.

require_once("../inc/boinc_db.inc");
require_once("../inc/util.inc");
require_once("../inc/user.inc");
require_once("../inc/consent.inc");

// Get the next url from POST
$next_url = post_str("next_url", true);
$next_url = urldecode($next_url);
$next_url = sanitize_local_url($next_url);
if (strlen($next_url) == 0) {
    $next_url = USER_HOME;
}

// validate checkbox
$agree = post_str("agree_to_terms_of_use", true);
if (!$agree) {
    error_page(tra("You have not agreed to our terms of use. You may not continue until you do so."));
}

// Obtain data from cookies
if (isset($_COOKIE['tempuserid'])) {
    $userid = $_COOKIE['tempuserid'];
}
else {
    error_page(tra("Website error when attempting to agree to terms of use. Please contact the site administrators."));
}
if (isset($_COOKIE['tempperm'])) {
    $perm = $_COOKIE['tempperm'];
}
else {
    error_page(tra("Website error when attempting to agree to terms of use. Please contact the site administrators."));
}
$user = BoincUser::lookup_id_nocache($userid);
$authenticator = $user->authenticator;

// Set consent in database, consent_type is ENROLL.
list($checkct, $ctid) = check_consent_type('ENROLL');
$rc1 = consent_to_a_policy($user, $ctid, 1, 0, 'Webform', time());
if (!$rc1) {
    error_page("Database error when attempting to INSERT into table consent with ID=$user->id. " . BoincDb::error() . " Please contact site administrators.");
}


// Log-in user
send_cookie('auth', $authenticator, $perm);
clear_cookie('tempuserid');
clear_cookie('tempperm');

// Send user to next_url
Header("Location: ".url_base()."$next_url");
?>
