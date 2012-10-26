<?php


function crumbs_build_trail($path) {
  $finder = _crumbs_get_finder();
  return $finder->buildTrail($path);
}


function crumbs_get_trail_finder() {
  return _crumbs_get_finder();
}


function _crumbs_get_finder() {
  static $_trailFinder;
  if (!isset($_trailFinder)) {
    $parent_finder = crumbs_get_parent_finder();
    $_trailFinder = new crumbs_TrailFinder($parent_finder);
  }
  return $_trailFinder;
}


function crumbs_get_parent_finder() {
  static $_parentFinder;
  if (!isset($_parentFinder)) {
    $plugin_engine = crumbs_get_plugin_engine();
    $_parentFinder = new crumbs_ParentFinder($plugin_engine);
  }
  return $_parentFinder;
}


/**
 * Very similar to menu_get_item(),
 * but with some little extra info stored in the result array.
 */
function crumbs_get_router_item($path) {
  $normalpath = drupal_get_normal_path($path);
  try {
    $item = menu_get_item($normalpath);
  }
  catch (Exception $e) {
    // Some modules throw an exception, if a path has unloadable arguments.
    // We don't care, because we don't actually load this page.
    // drupal_set_message('Ehm...', 'error');
    return NULL;
  }
  if (!empty($item) && is_array($item)) {
    // 'route' is a less ambiguous name for a router path than 'path'.
    $item['route'] = $item['path'];
    // 'href' sounds more like it had already run through url().
    $item['link_path'] = $normalpath;
    $item['alias'] = drupal_get_path_alias($normalpath);
    $item['fragments'] = explode('/', $normalpath);
    return $item;
  }
}


/**
 * Chop off path fragments until we find a valid path.
 * @param $path
 *   starting path or alias
 * @param $depth
 *   max number of fragments we try to chop off. -1 means no limit.
 */
function crumbs_reduce_path($path, $depth = -1) {
  $fragments = explode('/', $path);
  while (count($fragments) > 1 && $depth !== 0) {
    array_pop($fragments);
    $parent_path = implode('/', $fragments);
    $parent_item = crumbs_get_router_item($parent_path);
    if ($parent_item && $parent_item['href'] === $parent_item['link_path']) {
      return $parent_item['link_path'];
    }
    --$depth;
  }
}





