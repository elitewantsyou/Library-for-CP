struct Ukkonen {
public:
    Ukkonen() : global_end_(-1), last_pos_(0, 0) {
        nodes_.emplace_back();
        edges_.push_back({-2, -3, 0, 0});
    }

    struct edge {
        int left;
        int right;
        int from;
        int to;
    };

    struct Node {
        Node() : suf_link(0) {
        }
        int suf_link;
        std::map<char, int> go;
    };

    struct Pos {
        Pos(int len, int edge) : len(len), edge(edge) {
        }
        int len;
        int edge;
    };
    Pos Go(Pos pos, int ch) {
        if (pos.len == GetLen(edges_.at(pos.edge))) {
            if (nodes_.at(edges_.at(pos.edge).to).go.count(ch)) {
                return Pos(1, nodes_.at(edges_.at(pos.edge).to).go[ch]);
            }
            return Pos(-1, -1);
        }
//        char symb = str_[edges_[pos.edge].left + pos.len];
        char symb = str_.at(edges_.at(pos.edge).left + pos.len);
        if (symb == ch) {
            return Pos(pos.len + 1, pos.edge);
        }
        return Pos(-1, -1);
    }
    bool Check(const std::string &str) {
        Pos cur(0, 0);
        for (char c:str) {
            cur = Go(cur, c);
            if (cur.edge == -1) {
                return false;
            }
        }
        return true;
    }
    void Extend(char ch) {
        int ind = str_.size();
        str_.push_back(ch);
        ++global_end_;
        while (global_end_ >= 0) {
            auto new_pos = Go(last_pos_, ch);
            if (new_pos.len != -1) {
                last_pos_ = new_pos;
                return;
            }
            int ver = Split(last_pos_);
            Add(last_pos_, ind, -1);

            if (ver == 0) {
                last_pos_ = Pos(0, 0);
                return;
            }
            if (edges_.at(last_pos_.edge).from != 0) {
                last_pos_ = Shift(nodes_.at(edges_.at(last_pos_.edge).from).suf_link,
                                  edges_.at(last_pos_.edge).left,
                                  edges_.at(last_pos_.edge).left + last_pos_.len - 1);
            } else {
                if (last_pos_.len == 1) {
                    last_pos_ = Pos(0, 0);
                } else {
                    last_pos_ = Shift(nodes_.at(edges_.at(last_pos_.edge).from).suf_link,
                                      edges_.at(last_pos_.edge).left + 1,
                                      edges_.at(last_pos_.edge).left + last_pos_.len - 1);
                }
            }
            int res = Split(last_pos_);
            nodes_[ver].suf_link = res;
        }
    }
    // pass symbols from s[l] to s[r] inclusively.

    int Split(Pos pos) {
        if (pos.len == GetLen(edges_.at(pos.edge))) {
            return edges_.at(pos.edge).to;
        }
        int to = edges_.at(pos.edge).to;
        int new_ver = nodes_.size();
        nodes_.emplace_back();
        int new_edge = edges_.size();
        edges_.push_back({edges_.at(pos.edge).left + pos.len, edges_.at(pos.edge).right, new_ver,
                          to});
        nodes_.at(new_ver).go[str_.at(edges_.at(new_edge).left)] = new_edge;
        edges_.at(pos.edge).right = edges_.at(new_edge).left - 1;
        edges_.at(pos.edge).to = new_ver;
        return new_ver;
    }
    void Add(Pos pos, int left, int right) {
        int new_v = nodes_.size();
        nodes_.emplace_back();
        nodes_.at(edges_.at(pos.edge).to).go[str_.at(left)] = edges_.size();
        edges_.push_back({left, right, edges_.at(pos.edge).to, new_v});
    }
    int GetLen(const edge &edge) {
        if (edge.right == -1) {
            return global_end_ - edge.left + 1;
        }
        return edge.right - edge.left + 1;
    }
    Pos Shift(int ver, int left, int right) {
        int cur_len = 0;
        int len = right - left + 1;
        while (cur_len < len) {
            char c = str_.at(left + cur_len);
            const edge &ed = edges_.at(nodes_.at(ver).go[c]);
            int edge_len = GetLen(ed);
            if (cur_len + edge_len >= len) {
                return {len - cur_len, nodes_.at(ver).go[c]};
            } else {
                cur_len += edge_len;
                ver = ed.to;
            }
        }
        return Pos(-1, -1);
    }
    std::vector<Node> nodes_;
    std::vector<edge> edges_;
    std::string str_;
    int global_end_;
    Pos last_pos_;
};