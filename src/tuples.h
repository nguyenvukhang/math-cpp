template <typename T1, typename T2>
struct tup2 {
  T1 v1;
  T2 v2;
  tup2(const T1 &u1, const T2 &u2) : v1(u1), v2(u2) {}
  tup2(T1 &&u1, T2 &&u2) : v1(u1), v2(u2) {}

  template <class U1, class U2>
  tup2(const tup2<U1, U2> &p) : v1(p.v1), v2(p.v2) {}
};

template <typename T1, typename T2, typename T3>
struct tup3 {
  T1 v1;
  T2 v2;
  T3 v3;
  tup3(const T1 &u1, const T2 &u2, const T3 &u3) : v1(u1), v2(u2), v3(u3) {}
  tup3(T1 &&u1, T2 &&u2, T3 &&u3) : v1(u1), v2(u2), v3(u3) {}

  template <typename U1, typename U2, typename U3>
  tup3(const tup3<U1, U2, U3> &p) : v1(p.v1), v2(p.v2), v3(p.v3) {}
};

template <typename T1, typename T2, typename T3, typename T4>
struct tup4 {
  T1 v1;
  T2 v2;
  T3 v3;
  T4 v4;
  tup4(const T1 &u1, const T2 &u2, const T3 &u3, const T4 &u4)
      : v1(u1), v2(u2), v3(u3), v4(u4) {}
  tup4(T1 &&u1, T2 &&u2, T3 &&u3, T4 &&u4) : v1(u1), v2(u2), v3(u3), v4(u4) {}

  template <typename U1, typename U2, typename U3, typename U4>
  tup4(const tup4<U1, U2, U3, U4> &p)
      : v1(p.v1), v2(p.v2), v3(p.v3), v4(p.v4) {}
};
