<?php


class crumbs_TrailFinder {

  protected $parentFinder;

  function __construct($parent_finder) {
    $this->parentFinder = $parent_finder;
  }

  /**
   * Build the raw trail,
   * with no respect to title, access check, or skip-in-breadcrumb.
   */
  function buildTrail($path) {
    $path = drupal_get_normal_path($path);
    $trail_reverse = array();
    $front_normal_path = drupal_get_normal_path(variable_get('site_frontpage', 'node'));
    $front_menu_item = crumbs_get_router_item($front_normal_path);
    while (strlen($path) && $path !== '<front>' && $path !== $front_normal_path) {
      if (isset($trail_reverse[$path])) {
        // We found a loop! To prevent infinite recursion, we
        // remove the loopy paths from the trail and finish directly with <front>.
        while (isset($trail_reverse[$path])) {
          array_pop($trail_reverse);
        }
        break;
      }
      $item = crumbs_get_router_item($path);
      // if menu_get_item() does not resolve as a valid router item,
      // we skip this path.
      if ($item && $item['access']) {
        $trail_reverse[$path] = $item;
      }
      $parent_path = $this->parentFinder->getParentPath($path, $item);
      if ($parent_path === $path) {
        // This is again a loop, but with just one step.
        // Not as evil as the other kind of loop.
        break;
      }
      $path = $parent_path;
    }
    unset($trail_reverse['<front>']);
    $trail_reverse[$front_normal_path] = $front_menu_item;
    return array_reverse($trail_reverse);
  }
}
