<?php


function views_ui_crumbs_plugins() {
  // TODO: Port to D7 or drop.
  return;
  return new _views_ui_CrumbsPlugin();
}



/**
 * Implementation of class hook class_CrumbsParentFinder
 * on the behalf of menu module.
 */
class _views_ui_CrumbsPlugin {
  
  function define($h) {
    $h->setTitle(t('Define more meaningful breadcrumb titles for views_ui pages.'));
  }
  
  function findTitle__admin_build_views_export_x($path, $item) {
    $view = $item['map'][4];
    return t('Export view "@view"', array('@view' => $view->name));
  }
  
  function findTitle__admin_build_views_clone_x($path, $item) {
    $view = $item['map'][4];
    return t('Clone view "@view"', array('@view' => $view->name));
  }
  
  function findTitle__admin_build_views_edit_x($path, $item) {
    $view = $item['map'][4];
    return t('Edit view "@view"', array('@view' => $view->name));
  }
  
  function findTitle__admin_build_views_delete_x($path, $item) {
    $view = $item['map'][4];
    return t('Delete view "@view"', array('@view' => $view->name));
  }
}







