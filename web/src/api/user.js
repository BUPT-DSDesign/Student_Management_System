import service from '@utils/request'


export const login = (data) => {
    return service({
        url: "/user/login",
        method: 'post',
        data: data
    })
}

