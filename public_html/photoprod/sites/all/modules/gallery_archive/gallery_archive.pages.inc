<?php

/**
 * @file
 * Pages for Gallery Archives.
 */

/**
 * Callback for Gallery Archive page.
 */
function gallery_archive_gallery($node) {
  // Define some variables.
  $directory_name = $node->nid . '-' . time(); // nid + Current UNIX timestamp (must be unique)
  $tmp_directory = file_directory_temp() . '/' . $directory_name;
  $image_base_path = $_SERVER['DOCUMENT_ROOT'] . '/'; // Path to docroot
  $archive_name = $node->nid . '-archive'; // Name of final archive
  $file_directory_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files'); // Drupal's file directory path
  $archive_path = $file_directory_path . '/gallery-archives/' . $archive_name; // Used for building a link
  $tar_name = $archive_name . '.tar.gz';

  // TODO - Before creating a new archive, check if (a) the archive already
  // exists, and (b) if the archive is more than a week old. We'll need to
  // come up with a better solution for stale data here, though... but I want
  // to be efficient!

  if (!file_exists($archive_path . '.tar.gz')) {
    // Create a new temporary directory to store the images.
    if (!mkdir($tmp_directory, 0775)) {
      return '<div>' . t("Error: Temporary directory could not be created. Please check your file system settings.") . '</div>';
    }

    // Get a list of all the images to be added to the archive.
    $image_field = variable_get('gallery_archive_gallery_image_field', 0);
    $images = $node->media_gallery_media[LANGUAGE_NONE];

    // Copy all the full-resolution images into $tmp_directory.
    $total = count($images);
    foreach ($images as $key => $image) {
      $file = file_load($image['fid']);
      if ($key == 0) {
        $archive_file_name = preg_replace('/(.*)\./', 'front_cover.', $file->filename);
      } elseif ($key == $total - 1) {
        $archive_file_name = preg_replace('/(.*)\./', 'back_cover.', $file->filename);
      } else {
        $archive_file_name = preg_replace('/(.*)\./', str_pad((string)$key, 3, "0", STR_PAD_LEFT) . '.', $file->filename);
      }

      // Get the path to the full-resolution image.
      $image_to_copy = drupal_realpath($file->uri);

      // Copy image to $tmp_directory
      file_unmanaged_copy($image_to_copy, $tmp_directory . '/' . $archive_file_name, FILE_EXISTS_REPLACE);
    }
    $tar_file = new stdClass();
    $tar_file->uid = 1;
    $tar_file->filename = $tar_name;
    $tar_file->filemime = 'application/x-gzip';
    $tar_file->status = 1;
    $tar_file->uri = 'private://gallery-archives/' . $archive_name . '.tar.gz';
    file_save($tar_file);

    // @TODO - If PHP is compiled with --enable-zip, allow .zip archive...
    // Opt 1: http://davidwalsh.name/create-zip-php
    // Opt 2: http://www.webandblog.com/hacks/zip-a-folder-on-the-server-with-php/

    // Create the archive using our tgz function.
    gallery_archive_tgz_archive_dir($tmp_directory, $archive_name, TRUE);
  } else {
    $tar_fid = gallery_archive_get_fid($tar_name);
    $tar_file = file_load($tar_fid);
  }

  // Check to see if the Archive actually exists and has content.
  $archive_size = filesize($archive_path . '.tar.gz');

  // If it exists, return content with download link.
  if ($archive_size > 0) {
    $output = '<div>' . t("Click on the download link below to download this gallery:") . '</div>';
    $link_text = t("Download this gallery (!size)", array('!size' => _gallery_archive_format_bytes($archive_size)));
    $path = 'media/' . $tar_file->fid . '/download/' . $tar_file->filename;
    $output .= l($link_text, $path);
  }
  // Otherwise, show an error message.
  else {
    $output .= '<div>' . t("Error: No archive has been created. Please check your settings.") . '</div>';
  }

  return $output;
}

function gallery_archive_get_fid($filename) {
  $query = db_select('file_managed', 'f');
  $query
    ->fields('f', array('fid'))
    ->condition('f.filename', $filename,'=');
  $results = $query->execute()->fetchAssoc();
  return $results['fid'];
}
