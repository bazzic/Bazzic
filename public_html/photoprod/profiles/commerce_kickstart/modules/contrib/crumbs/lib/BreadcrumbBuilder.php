<?php


class crumbs_BreadcrumbBuilder {

  protected $pluginEngine;

  function __construct($pluginEngine) {
    $this->pluginEngine = $pluginEngine;
  }

  function buildBreadcrumb($trail) {
    $breadcrumb = array();
    foreach ($trail as $path => $item) {
      if ($item) {
        $title = $this->_findTitle($path, $item, $breadcrumb);
        if (!isset($title)) {
          $title = $item['title'];
        }
        // The item will be skipped, if $title === FALSE.
        if (isset($title) && $title !== FALSE && $title !== '') {
          $item['link_title'] = $title;
          $item['link_options'] = array();
          $breadcrumb[] = $item;
        }
      }
    }
    $this->_decorateBreadcrumb($breadcrumb);
    return $breadcrumb;
  }

  protected function _findTitle($path, array $item, array $breadcrumb_parents) {
    $invokeAction = new crumbs_InvokeAction_findTitle($path, $item, $breadcrumb_parents);
    $this->pluginEngine->invokeAll_find($invokeAction);
    return $invokeAction->getValue();
  }

  protected function _decorateBreadcrumb(array &$breadcrumb) {
    $invokeAction = new crumbs_InvokeAction_decorateBreadcrumb($breadcrumb);
    $this->pluginEngine->invokeAll_alter($invokeAction);
  }
}
