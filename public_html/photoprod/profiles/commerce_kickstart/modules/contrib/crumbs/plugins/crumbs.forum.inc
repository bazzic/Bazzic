<?php


/**
 * Implements hook_crumbs_plugins()
 */
function forum_crumbs_plugins($api) {
  // TODO: Port the forum plugins to D7.
  return;
  $forum_vid = variable_get('forum_nav_vocabulary', NULL);
  if ($forum_vid) {
    return array(
      'hierarchy' => new _forum_CrumbsPlugin__hierarchy(),
      'term_node' => new _forum_CrumbsPlugin__term_node($forum_vid),
      'term_name' => new _forum_CrumbsPlugin__term_name(),
      'admin'     => new _forum_CrumbsPlugin__admin(),
    );
  }
}


class _forum_CrumbsPlugin__hierarchy {

  /**
   * Terms get their parent terms as breadcrumb parent.
   * The method name matches the router path "taxonomy/term/%".
   */
  function findParent__forum($path, $item) {
    if (preg_match('#^forum/(\d+)$#', $path, $m)) {
      list(,$tid) = $m;
      $q = db_query($sql = "SELECT parent FROM {term_hierarchy} WHERE tid = %d", $tid);
      while ($row = db_fetch_object($q)) {
        if ($row->parent) {
          return 'forum/'. $row->parent;
        }
      }
      return 'forum';
    }
    return '<front>';
  }
}


class _forum_CrumbsPlugin__term_node {

  protected $_forum_vid;

  function __construct($forum_vid) {
    $this->_forum_vid = $forum_vid;
  }

  /**
   * Forum nodes get their forum terms as breadcrumb parents.
   * The method name matches the router path "node/%".
   */
  function findParent__node_x($path, $item) {
    $node = $item['map'][0];
    if ($node->type == 'forum' && is_array($node->taxonomy)) {
      $result = array();
      foreach ($node->taxonomy as $tid => $term) {
        if ($term->vid == $this->_forum_vid) {
          return 'forum/'. $tid;
        }
      }
    }
  }
}


class _forum_CrumbsPlugin__term_name {

  function findTitle__forum($path, $item) {
    if (preg_match('#^forum/(\d+)$#', $path, $m)) {
      list(,$tid) = $m;
      $q = db_query($sql = "SELECT name FROM {term_data} WHERE tid = %d", $tid);
      if ($row = db_fetch_object($q)) {
        return $row->name;
      }
    }
    return t('Forums');
  }
}


class _forum_CrumbsPlugin__admin {

  protected $_containers;

  function findTitle__admin_content_forum($path, $item) {
    switch ($path) {
      case 'admin/content/forum/edit':
      case 'admin/content/forum/add':
        // skip
        return FALSE;
    }
  }



  function findTitle__admin_content_forum_edit_forum_x($path, $item) {
    $term = $item['map'][5];
    return $term['name'];
  }

  function findTitle__admin_content_forum_edit_container_x($path, $item) {
    $term = $item['map'][5];
    return $term['name'];
  }

  protected function _isContainer($tid) {
    if (!isset($this->_containers)) {
      $containers = variable_get('forum_containers', array());
      $this->_containers = array_combine($containers, $containers);
    }
    return isset($this->_containers[$tid]) ? TRUE : FALSE;
  }
}







